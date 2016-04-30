
#ifndef _SLANK_H_
#define _SLANK_H_

#include <linux/ioctl.h>
#include <linux/cdev.h>


#ifndef SLANK_MAJOR
#define SLANK_MAJOR 0
#endif

#ifndef SLANK_NR_DEVS
#define SLANK_NR_DEVS 4
#endif

struct slank_dev {
    uint8_t* data; 
    // struct semaphore sem;
    struct cdev cdev;
};


extern int slank_major;

int slank_p_init(dev_t dev);
void slank_p_cleanup(void);

int slank_read(struct file* file, char __user *buf, size_t count, loff_t *f_pos);
int slank_write(struct file* file, const char __user *buf, size_t count, loff_t *f_pos);

#endif /* _SLANK_H_ */
