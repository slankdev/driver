

#include <linux/init.h>
#include <linux/module.h>

#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#ifndef SLANK_MAJOR
#define SLANK_MAJOR 0
#endif


MODULE_LICENSE("Dual BSD/GPL");

struct cdev slank_cdev;


int slank_major   = SLANK_MAJOR;
int slank_minor   = 0;
int slank_nr_devs = 1;

module_param(slank_major  , int, S_IRUGO);
module_param(slank_minor  , int, S_IRUGO);
module_param(slank_nr_devs, int, S_IRUGO);




int slank_open(struct inode* inode, struct file* filp)
{
    return 0;
}


int slank_release(struct inode* inode, struct file* filp)
{
    return 0;
}

ssize_t slank_read(struct file* filp, char __user* buf, size_t count, 
        loff_t *f_pops)
{
    return 0;
}

ssize_t slank_write(struct file* filp, const char __user* buf, size_t count, 
        loff_t *f_pops)
{
    return 0;
}



struct file_operations slank_fops = {
    .owner   = THIS_MODULE, 
    .read    = slank_read,
    .write   = slank_write,
    .open    = slank_open,
    .release = slank_release,
};




static void slank_cleanup_module(void)
{
    dev_t devno = MKDEV(slank_major, slank_minor);

    cdev_del(&slank_cdev);
    unregister_chrdev_region(devno, slank_nr_devs);

	printk(KERN_ALERT "Goodbye, slankworld\n");
}




static int slank_init_module(void)
{
    int result, i;
    dev_t dev;
    
	printk(KERN_ALERT "I'm slankdev. Nice to meet you.\n");
    
    if (SLANK_MAJOR) {
        dev = MKDEV(slank_major, slank_minor);
        result = register_chrdev_region(dev, slank_nr_devs, "slank");
        printk(KERN_NOTICE "slank: major number is %d static allocated\n", slank_major);
    } else {
        result = alloc_chrdev_region(&dev, slank_minor, slank_nr_devs, "slank");
        slank_major = MAJOR(dev);
        printk(KERN_NOTICE "slank: major number is %d dynamic allocated\n", slank_major);
    }

    if (result < 0) {
        printk(KERN_WARNING "slank: can't get major %d\n", slank_major);
        return result;
    }

    for (i=0; i<slank_nr_devs; i++) {
        cdev_init(&slank_cdev, &slank_fops);
        slank_cdev.owner = THIS_MODULE;
        slank_cdev.ops = &slank_fops;

        result = cdev_add(&slank_cdev, dev, 1);
        if (result) {
            printk(KERN_NOTICE "Error %d adding slank%d", result, i);
            goto fail;
        }
    }   

	return 0; /* succeed */

fail:
    slank_cleanup_module();
    return result;
}


module_init(slank_init_module);
module_exit(slank_cleanup_module);
