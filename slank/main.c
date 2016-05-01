

#include <linux/init.h>
#include <linux/module.h>

#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/slab.h>
/* #include <linux/uaccess.h> */

#include "slank.h"

MODULE_LICENSE("Dual BSD/GPL");


int slank_major;
int slank_minor   = 0;
int slank_nr_devs = SLANK_NR_DEVS;

module_param(slank_major  , int, S_IRUGO);
module_param(slank_minor  , int, S_IRUGO);
module_param(slank_nr_devs, int, S_IRUGO);

struct slank_dev* slank_devices;




/* int slank_trim(struct slank_dev* dev) */
/* { */
/*     return 0; */
/* } */



int slank_open(struct inode* inode, struct file* filp)
{
    struct slank_dev* dev;

    dev = container_of(inode->i_cdev, struct slank_dev, cdev);
    filp->private_data = dev;

    return 0;
}


int slank_release(struct inode* inode, struct file* filp)
{
    return 0;
}



/* ssize_t slank_read(struct file* filp, char __user* buf, size_t count,  */
/*         loff_t *f_pops) */
/* { */
/*     return 1; */
/* } */



/* ssize_t slank_write(struct file* filp, const char __user* buf, size_t count,  */
/*         loff_t *f_pops) */
/* { */
/*     return 1; */
/* } */



/* loff_t slank_llseek(struct file* filp, loff_t off, int whence) */
/* { */
/*     return 1; */
/* } */
/*  */



struct file_operations slank_fops = {
    .owner   = THIS_MODULE, 
    /* .llseek  = slank_llseek, */
    /* .read    = slank_read, */
    /* .write   = slank_write, */
    .open    = slank_open,
    .release = slank_release,
};




static void slank_cleanup_module(void)
{
    int i;
    dev_t devno = MKDEV(slank_major, slank_minor);

    if (slank_devices) {
        for (i=0; i<slank_nr_devs; i++) {
            cdev_del(&slank_devices[i].cdev);
        }
        kfree(slank_devices);
    }
    unregister_chrdev_region(devno, slank_nr_devs);

	printk(KERN_ALERT "Goodbye, slankworld\n");
}





static void slank_setup_cdev(struct slank_dev* dev, int index)
{
    int err;
    int devno = MKDEV(slank_minor, slank_minor+index);

    cdev_init(&(dev->cdev), &slank_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &slank_fops;
    
    err = cdev_add(&(dev->cdev), devno, 1);
    if (err)
        printk(KERN_NOTICE "Error %d adding slank%d", err, index);
}



static int slank_init_module(void)
{

    int result, i;
    dev_t dev = 0;
    
	printk(KERN_ALERT "I'm slankdev. Nice to meet you.\n");

    /* Alloc major device number */
    result = alloc_chrdev_region(&dev, slank_minor, slank_nr_devs, "slank");
    slank_major = MAJOR(dev);
    printk(KERN_NOTICE "slank: major number is %d dynamic allocated\n", slank_major);

    if (result < 0) {
        printk(KERN_WARNING "slank: can't get major %d\n", slank_major);
        return result;
    }

    slank_devices = kmalloc(slank_nr_devs * sizeof(struct slank_dev), GFP_KERNEL);
    if (!slank_devices) {
        printk(KERN_WARNING "slank: can't kmalloc\n");
        result = -ENOMEM;
        goto fail;
    }
    memset(slank_devices, 0, slank_nr_devs * sizeof(struct slank_dev));

    for (i=0; i<slank_nr_devs; i++) {
        slank_setup_cdev(&slank_devices[i], i);
    }   

	return 0; /* succeed */

fail:
    slank_cleanup_module();
    return result;
}


module_init(slank_init_module);
module_exit(slank_cleanup_module);
