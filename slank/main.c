

#include <linux/init.h>
#include <linux/module.h>

#include <linux/types.h>
#include <linux/fs.h>

#include "slank.h"

MODULE_LICENSE("Dual BSD/GPL");


int slank_major   = SLANK_MAJOR;
int slank_minor   = 0;
int slank_nr_devs = SLANK_NR_DEVS;


static void slank_cleanup_module(void)
{
	printk(KERN_ALERT "Goodbye, slankworld\n");
}

static int slank_init_module(void)
{

    int result, i;
    dev_t dev = 0;
    
	printk(KERN_ALERT "I'm slankdev. Nice to meet you.\n");

    if (slank_major) { /* This booling is always false */
        printk(KERN_ALERT "use register_chrdev_region()\n");
        dev = MKDEV(slank_major, slank_minor);
        result = register_chrdev_region(dev, slank_nr_devs, "slank");
    } else {
        printk(KERN_ALERT "use alloc_chrdev_region()\n");
        result = alloc_chrdev_region(&dev, slank_minor, slank_nr_devs, "slank");
        slank_major = MAJOR(dev);
    }

    if (result < 0) {
        printk(KERN_WARNING "slank: can't get major %d\n", slank_major);
        return result;
    }

    i = 100;
	return 0;
}


module_init(slank_init_module);
module_exit(slank_cleanup_module);
