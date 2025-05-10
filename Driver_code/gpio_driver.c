#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/cdev.h>

#define DEVICE_NAME "gpio_ioctl_device"
#define CLASS_NAME "gpio_ioctl"

// IOCTL definitions
#define GPIO_MAGIC 'G'
#define GPIO_SET_PIN _IOW(GPIO_MAGIC, 0, int)
#define GPIO_GET_PIN _IOR(GPIO_MAGIC, 1, int)
#define GPIO_RESET_PIN _IOW(GPIO_MAGIC, 2, int)

static int major_number;
static struct class* gpio_class = NULL;
static struct device* gpio_device = NULL;
static int device_open_count = 0;

static int gpio_open(struct inode *inodep, struct file *filep) {
    if (device_open_count > 0)
        return -EBUSY;
    device_open_count++;
    try_module_get(THIS_MODULE);
    printk(KERN_INFO "GPIO device opened\n");
    return 0;
}

static int gpio_release(struct inode *inodep, struct file *filep) {
    device_open_count--;
    module_put(THIS_MODULE);
    printk(KERN_INFO "GPIO device closed\n");
    return 0;
}

static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int gpio_pin;
    int value;

    switch (cmd) {
        
        case GPIO_SET_PIN:
            if (copy_from_user(&gpio_pin, (int __user *)arg, sizeof(gpio_pin)))
                return -EFAULT;
            if (gpio_direction_output(gpio_pin, 1) < 0)
                return -EINVAL;
            gpio_set_value(gpio_pin, 1);
            printk(KERN_INFO "GPIO %d set to high\n", gpio_pin);
            break;

        case GPIO_RESET_PIN:
            if (copy_from_user(&gpio_pin, (int __user *)arg, sizeof(gpio_pin)))
                return -EFAULT;
            if (gpio_request(gpio_pin, "GPIO_RESET_PIN") < 0)  // Yêu cầu quyền truy cập GPIO
                return -EINVAL;
            if (gpio_direction_output(gpio_pin, 0) < 0)
                return -EINVAL;
            gpio_set_value(gpio_pin, 0);
            printk(KERN_INFO "GPIO %d set to low\n", gpio_pin);
            break;

            case GPIO_GET_PIN:
            if (copy_from_user(&gpio_pin, (int __user *)arg, sizeof(gpio_pin)))
                return -EFAULT;
            if (gpio_direction_input(gpio_pin) < 0)
                return -EINVAL;
            value = gpio_get_value(gpio_pin);
            if (copy_to_user((int __user *)arg, &value, sizeof(value)))
                return -EFAULT;
            printk(KERN_INFO "GPIO %d read: %d\n", gpio_pin, value);
            break;

        default:
            return -ENOTTY;
    }


    return 0;
}


static struct file_operations fops = {
    .open = gpio_open,
    .release = gpio_release,
    .unlocked_ioctl = gpio_ioctl,
};

static int __init gpio_ioctl_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    gpio_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(gpio_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(gpio_class);
    }
    gpio_device = device_create(gpio_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(gpio_device)) {
        class_destroy(gpio_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(gpio_device);
    }
    printk(KERN_INFO "GPIO ioctl device registered correctly\n");
    return 0;
}

static void __exit gpio_ioctl_exit(void) {
    device_destroy(gpio_class, MKDEV(major_number, 0));
    class_unregister(gpio_class);
    class_destroy(gpio_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "GPIO ioctl device unregistered\n");
}

module_init(gpio_ioctl_init);
module_exit(gpio_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDT");
MODULE_DESCRIPTION("A simple Linux char driver for GPIO control using ioctl");
