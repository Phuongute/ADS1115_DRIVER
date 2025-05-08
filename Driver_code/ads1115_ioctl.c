//#https://www.kernel.org/doc/html/v4.14/driver-api/i2c.html
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#define DRIVER_NAME "ads1115_driver"
#define CLASS_NAME "ads1115"
#define DEVICE_NAME "ads1115"

#define ADS1115_ADR 0x48    //ADDR nối với Ground
#define ADS_REG_CONV 0x00
#define ADS_REG_CONFIG 0x01
#define ADS_REG_LO_THRESH 0x02
#define ADS_REG_HI_THRESH 0x03

static struct   i2c_client *ads1115_client;
static struct   class* ads1115_class = NULL;
static struct   device* ads1115_device = NULL;
static int      major_number;

// static int ads1115_set_config(u16 config)
// {
//     int ret;
//     ret = i2c_smbus_write_word_data(ads1115_client, ADS_REG_CONFIG, cpu_to_be16(config));
//     if (ret < 0) {
//         pr_err("ADS1115: Failed to write config: %d\n", ret);
//         return ret;
//     }
// }
static long ads1115_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("LAPOLICE!!\n");
    return 0;
}

static int ads1115_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ads1115 device opened\n");
    return 0;
}

static int ads1115_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ads1115 device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = ads1115_open,
    .unlocked_ioctl = ads1115_ioctl,
    .release = ads1115_release,
};
static int ads1115_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    ads1115_client = client;

    // Create a char device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ERR "Failed to register a major number\n");
        return major_number;
    }

    ads1115_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ads1115_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to register device class\n");
        return PTR_ERR(ads1115_class);
    }

    ads1115_device = device_create(ads1115_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ads1115_device)) {
        class_destroy(ads1115_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to create the device\n");
        return PTR_ERR(ads1115_device);
    }

    printk(KERN_INFO "ADS1115 driver installed\n");
    return 0;
}


static int ads1115_remove(struct i2c_client *client)
{
    device_destroy(ads1115_class, MKDEV(major_number, 0));
    class_unregister(ads1115_class);
    class_destroy(ads1115_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "ADS1115 driver removed\n");
    return 0;
}


static const struct of_device_id ads1115_of_match[] = {
    { .compatible = "TI,ads1115", },
    { },
};
MODULE_DEVICE_TABLE(of, ads1115_of_match);

static struct i2c_driver ads1115_driver = {
    .driver = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(ads1115_of_match),
    },
    .probe      = ads1115_probe,
    .remove     = ads1115_remove,
};

static int __init ads1115_init(void)
{
    printk(KERN_INFO "Initializing ADS1115 driver\n");
    return i2c_add_driver(&ads1115_driver);
}

static void __exit ads1115_exit(void)
{
    printk(KERN_INFO "Exiting ADS1115 driver\n");
    i2c_del_driver(&ads1115_driver);
}

module_init(ads1115_init);
module_exit(ads1115_exit);

MODULE_AUTHOR("...");
MODULE_DESCRIPTION("ADS1115 I2C Client Driver");
MODULE_LICENSE("GPL");
