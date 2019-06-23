#include <linux/fs.h>				// file_operations declaration
#include <linux/module.h>			// module_init module_exit declaration
#include <linux/init.h>				// __init __exit declaration
#include <linux/device.h>			// class device declaration
#include <linux/uaccess.h>			// copy_from_user header
#include <linux/types.h>			// device id dev_t type declaration
#include <asm/io.h>				// ioremap iounmap header

static struct class *leddrv_class;
static struct device *leddrv_class_dev;

static dev_t devno;				// device number
static int major = 231;				// major device number
static int minor = 0;				// minor device number
static char *module_name = "led_drv";		// module name

volatile unsigned long *gpfsel0 = NULL;		// function selector register pointer
volatile unsigned long *gpset0 = NULL;		// set to high volt level register pointer
volatile unsigned long *gpclr0 = NULL;		// set to low volt level register pointer

// led_open function
static int led_open(struct inode *inode, struct file *file)
{
	// printk("led_open\n");
	// Configure GPIO2 Output 001 GPIO Pin2 is an output
	*gpfsel0 &= ( ~( 0x6 << ( 2 * 3 ) ) );
	*gpfsel0 |= ( 0x1 << ( 2 * 3 ) );
	return 0;
}

// led_write function
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	int val = 0;
	copy_from_user(&val, buf, count);
	if(val == 1)
	{
		// led on
		printk("led on\n");
		*gpset0 |= (0x1 << 2);
	}
	else
	{
		// led off
		printk("led off\n");
		*gpclr0 |= (0x1 << 2);
	}
	return 0;
}

int __init led_drv_init(void)
{
	int ret;
	devno = MKDEV(major, minor);
	ret = register_chrdev(major, module_name, &leds_fops);

	leddrv_class = class_create(THIS_MODULE, "myfirstdemo");

	leddrv_class_dev = device_create(leddrv_class, NULL, devno, NULL, module_name);

	gpfsel0 = (volatile unsigned long *)ioremap(0x3f200000, 4);
	gpset0 = (volatile unsigned long *)ioremap(0x3f20001c, 4);
	gpclr0 = (volatile unsigned long *)ioremap(0x3f200028, 4);
	return 0;
}

void __exit led_drv_exit(void)
{
	device_destroy(leddrv_class, devno);
	class_destroy(leddrv_class);

	unregister_chrdev(major, module_name);
	iounmap(gpfsel0);
	iounmap(gpset0);
	iounmap(gpclr0);
}

module_init(led_drv_init);
module_exit(led_drv_exit);
MODULE_LICENSE("GPL v2");

