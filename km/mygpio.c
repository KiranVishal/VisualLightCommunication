#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/arch/pxa-regs.h>
#include <asm-arm/arch/hardware.h>
#include <asm/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

//#define GPIO_LED0 28 // Pin2 - AC

static int mygpio_major = 61;

static unsigned int value;

static int mygpio_open(struct inode *inode, struct file *filp);
static int mygpio_release(struct inode *inode, struct file *filp);
static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);

struct file_operations mygpio_fops = {
    write: mygpio_write,
    read: mygpio_read,
    open: mygpio_open,
    release: mygpio_release,
};

static int my_init_module(void)
{
	int result;

	// INIT character device	
    result = register_chrdev(mygpio_major, "mygpio", &mygpio_fops);
    if (result < 0)
    {
        printk(KERN_ALERT
                "mygpio: cannot obtain major number %d\n", mygpio_major);
        return -1;
    }
	// Init LEDs
	CKEN |= CKEN0_PWM0;

	pxa_gpio_mode(GPIO16_PWM0_MD);
	
	PWM_CTRL0 = 64;
	PWM_PWDUTY0 = 1;
	PWM_PERVAL0 = 1;

	//printk("%d : %d : %d : %d\n", CKEN, PWM_CTRL0, PWM_PWDUTY0, PWM_PERVAL0);
	return 0;
}

static void my_cleanup_module(void)
{
	CKEN &= ~CKEN0_PWM0;

	PWM_CTRL0 = 0;
	PWM_PWDUTY0 = 0;
	PWM_PERVAL0 = 0;

	unregister_chrdev(mygpio_major, "mygpio");

}

static int mygpio_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int mygpio_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char input_buf[count];

	memset(input_buf, 0, sizeof(char)*count);

	if (copy_from_user(input_buf, buf, count))
	{
		return -EFAULT;
	}

	sscanf(input_buf, "%u", &value);

	//printk("input_buf:%s, value:%u\n", input_buf, value);

	if (value > 255)
		value = 255;

	//SET PWM based on value
	PWM_PWDUTY0 = 2*value;
	PWM_PERVAL0 = 512;//2*(256-value);

	return count;
}

static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{

	char output[100];
	memset(output, 0, 100);

	if (*f_pos != 0)
	{
		*f_pos = 0;
		return 0;
	}

	sprintf(output, "%u\n", value);

	if (copy_to_user(buf, output, strlen(output)))
	{
		return -EFAULT;
	}

	count = strlen(output);
	*f_pos = count;

	return count;
}

module_init(my_init_module);
module_exit(my_cleanup_module);
