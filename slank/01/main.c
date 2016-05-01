

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


struct node {
    uint8_t *data;
    size_t   len;
    struct node *next;
};
struct slank_dev {
    struct node* head;
    struct cdev cdev;
};

struct slank_dev *slank_devices;

int slank_major   = SLANK_MAJOR;
int slank_minor   = 0;

module_param(slank_major  , int, S_IRUGO);
module_param(slank_minor  , int, S_IRUGO);



static struct node *alloc_node(size_t size)
{
    struct node *node = kmalloc(sizeof(struct node), GFP_KERNEL);
    node->len  = size;
    node->data = kmalloc(node->len, GFP_KERNEL);
    node->next = NULL;
    return node;
}

static struct node* get_tail(void)
{
    struct node* n = slank_devices->head;
    while (1) {
        if (n) {
            if (n->next)
                n = n->next;
            else 
                break;
        } else {
            break;
        }
    }
    return n;
}


static void add_tail(size_t len)
{
    struct node* tail = get_tail();
    if (tail) {
        tail->next = alloc_node(len);
    } else {
        slank_devices->head = alloc_node(len);
    }
}


static void rm_head(void)
{
    if (slank_devices->head) {
        struct node* next = slank_devices->head->next;
        kfree(slank_devices->head->data);
        kfree(slank_devices->head);
        slank_devices->head = next;
    }
    return;
}

int slank_open(struct inode* inode, struct file* filp)
{
    /* slank_devices->head = NULL; */
    return 0;
}


int slank_release(struct inode* inode, struct file* filp)
{
    /* while (1) { */
    /*     if (slank_devices->head == NULL) */
    /*         break; */
    /*     rm_head(); */
    /* } */
    return 0;
}

ssize_t slank_read(struct file* filp, char __user* buf, size_t count, 
        loff_t *f_pops)
{
    struct node* n = slank_devices->head;
    if (n) {
        if (count > n->len)
            count = n->len;

        copy_to_user(buf, n->data, count);
        rm_head();
    } else {
        count = 0;
    }
    return count;
}

ssize_t slank_write(struct file* filp, const char __user* buf, size_t count, 
        loff_t *f_pops)
{
    add_tail(count);
    struct node* n = get_tail();
    copy_from_user(n->data, buf, count);
    return count;
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
    /* int i; */
    dev_t devno = MKDEV(slank_major, slank_minor);

    cdev_del(&slank_devices->cdev);
    kfree(slank_devices);

    unregister_chrdev_region(devno, 1);

	printk(KERN_ALERT "Goodbye, slankworld\n");
}



static int slank_init_module(void)
{
    int result;
    dev_t dev;
	printk(KERN_ALERT "I'm slankdev. Nice to meet you.\n");
    

    /* Allocate device major and minor number */
    if (SLANK_MAJOR) {
        dev = MKDEV(slank_major, slank_minor);
        result = register_chrdev_region(dev, 1, "slank");
        printk(KERN_NOTICE "slank: major number is %d static allocated\n", slank_major);
    } else {
        result = alloc_chrdev_region(&dev, slank_minor, 1, "slank");
        slank_major = MAJOR(dev);
        printk(KERN_NOTICE "slank: major number is %d dynamic allocated\n", slank_major);
    }
    if (result < 0) {
        printk(KERN_WARNING "slank: can't get major %d\n", slank_major);
        return result;
    }


    slank_devices = kmalloc(sizeof(struct slank_dev), GFP_KERNEL);
    if (!slank_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(slank_devices, 0, sizeof(struct slank_dev));


    cdev_init(&slank_devices->cdev, &slank_fops);
    slank_devices->cdev.owner = THIS_MODULE;
    slank_devices->cdev.ops   = &slank_fops;

    result = cdev_add(&slank_devices->cdev, dev, 1);
    if (result) {
        printk(KERN_NOTICE "Error %d adding slank", result);
        goto fail;
    }

    slank_devices->head = NULL;
	return 0; /* succeed */

fail:
    slank_cleanup_module();
    return result;
}


module_init(slank_init_module);
module_exit(slank_cleanup_module);
