#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/delay.h>

#define DRIVER_NAME "bh1750"
#define DRIVER_CLASS "bh1750Class"


dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static struct i2c_adapter * bh1750_i2c_adapter = NULL;
static struct i2c_client * bh1750_i2c_client = NULL;

/* Meta Information */
MODULE_AUTHOR("HaeYoung Jung");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out a bh1750 Ambient Light Sensor");


/* Defines for device identification */ 
#define I2C_BUS_AVAILABLE	1		/* The I2C Bus available on the raspberry */
#define SLAVE_DEVICE_NAME	"bh1750"	/* Device and Driver Name */
#define bh1750_SLAVE_ADDRESS	0x23		/* bh1750 I2C address */

#define BH1750_POWER_DOWN 0x00
#define BH1750_POWER_ON 0x01
#define BH1750_RESET 0x07
#define BH1750_CONTINUOUS_MEASUREMENT 0x10
#define Continuously_H_Resolution_Mode2 0x11
#define Continuously_L_Resolution_Mode 0x13
static const struct i2c_device_id bh1750_id[] = {
		{ SLAVE_DEVICE_NAME, 0 }, 
		{ }
};
MODULE_DEVICE_TABLE(i2c, bh1750_id);

static int bh1750_init(struct i2c_client *client)
{
	int ret;
    // Send the power-on command to the BH1750 sensor
    char cmd_power_on = BH1750_POWER_ON;
    ret = i2c_master_send(client, &cmd_power_on, 1);
    if (ret < 0) {
        printk(KERN_ERR "Error sending power-on command to BH1750 sensor\n");
        return ret;
    }

    // Set the measurement mode to high-resolution mode
    char cmd_set_mode = Continuously_L_Resolution_Mode;
    ret = i2c_master_send(client, &cmd_set_mode, 1);
    if (ret < 0) {
        printk(KERN_ERR "Error setting measurement mode of BH1750 sensor\n");
        return ret;
    }

    return 0;
}

static ssize_t bh1750_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    char data[2];
	static int bh1750_data = 0;
    // BH1750 센서로부터 데이터 읽기
    ret = i2c_master_recv(bh1750_i2c_client, data, 2);
    if (ret < 0) {
        printk(KERN_ERR "Error reading data from BH1750 sensor\n");
        return ret;
    }
	// Wait for conversion to complete (depends on mode)
    msleep(180); // You may need to adjust this based on your mode selection

    // 데이터를 16비트로 변환하여 bh1750_data에 저장
    bh1750_data = (data[0] << 8) | data[1];

    // 사용자 공간으로 데이터 전송
    ret = copy_to_user(buf, &bh1750_data, sizeof(bh1750_data));
    if (ret != 0) {
        printk(KERN_ERR "Failed to copy data to user space\n");
        return -EFAULT;
    }
	printk("read\n");
    return sizeof(bh1750_data);
}

	
static int bh1750_probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{
    	
	// BH1750 센서 초기화 및 설정
    int ret = bh1750_init(client);
    if (ret < 0) {
        printk(KERN_ERR "Failed to initialize BH1750 sensor\n");
        return ret;
    }	
	printk("probe\n");
    return 0;
}

static void bh1750_remove(struct i2c_client *client)
{
    // 디바이스 파일 해제
    unregister_chrdev(0, "bh1750");//Device Nr 247463936 was registered

    
}


static struct i2c_driver bh1750_driver = {
	.driver = {
		.name = SLAVE_DEVICE_NAME,
		.owner = THIS_MODULE
	},
	.remove = bh1750_remove,
	.probe = bh1750_probe,	
	.id_table = bh1750_id,
};

static struct i2c_board_info bh1750_i2c_board_info = {
	I2C_BOARD_INFO(SLAVE_DEVICE_NAME, bh1750_SLAVE_ADDRESS)	
};
static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = bh1750_read  
};
int __init ModuleInit(void) {
	
		/*Allocating Major number*/
	if((alloc_chrdev_region(&dev, 0, 1, "bh1750")) <0){
	pr_err("Cannot allocate major number\n");
	return -1;
	}
	pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

	/*Creating cdev structure*/
	cdev_init(&etx_cdev,&fops);

	/*Adding character device to the system*/
	if((cdev_add(&etx_cdev,dev,1)) < 0){
	pr_err("Cannot add the device to the system\n");
	return -1;
	}

	/*Creating struct class*/
	if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
	pr_err("Cannot create the struct class\n");
	return -1;
	}

	/*Creating device*/
	if((device_create(dev_class,NULL,dev,NULL,"bh1750")) == NULL){
	pr_err( "Cannot create the Device \n");
	return -1;
	}
	
	int ret = -1;	
	printk("MyDeviceDriver - Hello Kernel\n");
	
    bh1750_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (bh1750_i2c_adapter == NULL) {
        printk(KERN_ERR "Failed to get I2C adapter\n");
        goto ExitError;
    }

    bh1750_i2c_client = i2c_new_client_device(bh1750_i2c_adapter, &bh1750_i2c_board_info);
    if (bh1750_i2c_client == NULL) {
        printk(KERN_ERR "Failed to create BH1750 I2C client\n");
        goto ExitError;
    }

    // BH1750 센서 초기화 및 설정
	/*
    ret = bh1750_init(bh1750_i2c_client);
    if (ret < 0) {
        printk(KERN_ERR "Failed to initialize BH1750 sensor\n");
        goto ExitError;+
    }
	*/

    // BH1750 I2C 드라이버 등록
    ret = i2c_add_driver(&bh1750_driver);
    if (ret < 0) {
        printk(KERN_ERR "Failed to add BH1750 I2C driver\n");
        goto ExitError;
    }

    printk("BH1750 Driver added!\n");
    return 0;

ExitError:
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    return -1;
}

/**
 * @brief function, which is called when removing module from kernel
 * free alocated resources
 */
void __exit ModuleExit(void) {
	printk("MyDeviceDriver - Goodbye, Kernel!\n");
    i2c_unregister_device(bh1750_i2c_client);
    i2c_del_driver(&bh1750_driver);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);
