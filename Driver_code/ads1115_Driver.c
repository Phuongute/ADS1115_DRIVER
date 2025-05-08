#https://www.kernel.org/doc/html/v4.14/driver-api/i2c.html
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#define DRIVER_NAME "ads1115_driver"

#define ADS1115_ADR 0x48    //ADDR nối với Ground
#define ADS_REG_CONV 0x00
#define ADS_REG_CONFIG 0x01
#define ADS_REG_LO_THRESH 0x02
#define ADS_REG_HI_THRESH 0x03




static struct i2c_client *ads1115_client;

static int ads1115_read
static int ads1115_set_config(u16 config)
{
    int ret;
    ret = i2c_smbus_write_word_data(ads1115_client, ADS_REG_CONFIG, cpu_to_be16(config));
    if (ret < 0) {
        pr_err("ADS1115: Failed to set config: %d\n", ret);
        return ret;
    }
}
static int ads1115_read_adc(void)
{
    int ret
    int adcVal
    ret = i2c_smbus_read_word_data(ads1115_client,ADS_REG_CONV)
    if (ret < 0) {
        printk("ADS1115: Failed to read ADC value: %d\n", ret);
        return ret
    }
    adcVal = be16_to_cpu(ret);
    return (s16)adcVal;
    
}
static int ads1115_set_loThresh(u16 loThresh){
    int ret
    ret = i2c_smbus_write_word_data(ads1115_client,ADS_REG_LO_THRESH,cpu_to_be16(loThresh))
    if (ret < 0) {
        printk("ADS1115: Failed to set low thresh: %d\n", ret);
        return ret;
    }
}
static int ads1115_set_hiThresh(u16 hiThresh){
    int ret
    ret = i2c_smbus_write_word_data(ads1115_client,ADS_REG_HI_THRESH,cpu_to_be16(hiThresh))
    if (ret < 0) {
        printk("ADS1115: Failed to set hi thresh: %d\n", ret);
        return ret;
    }
}
static int ads1115_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    u16 conf = 0xC383; // 1100 0011 1000 0011
    ads1115_client = client;
    ret = i2c_smbus_write_word_data(client, ADS_REG_CONFIG, cpu_to_be16(conf) );
    if (ret < 0) {
        printk(KERN_ERR "ADS1115: Failed to config \n");
        return ret;
    }


    // Read data from MPU6050 sensor
    ret = i2c_smbus_read_word_data(client, 0x00);
    if (ret < 0) {
        printk(KERN_ERR "Failed to read Conversion Register from ADS1115\n");
        return ret;
    }

    printk(KERN_INFO "ADS1115: driver installed\n");

    return 0;
}


static void ads1115_remove(struct i2c_client *client)
{
    printk(KERN_INFO "ADS1115: driver removed\n");

    // Clean up
 
}

static const struct i2c_device_id ads1115_id[] = {
    { "ads1115", 0 },
    { }
};

MODULE_DEVICE_TABLE(i2c, ads1115_id);

static struct i2c_driver ads1115_driver = {
    .driver = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
    },
    .probe      = ads1115_probe,
    .remove     = ads1115_remove,
    .id_table   = ads1115_id,
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