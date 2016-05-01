
#ifndef _SLANK_H_
#define _SLANK_H_

#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>


#ifndef SLANK_NR_DEVS
#define SLANK_NR_DEVS 4
#endif


struct slank_dev {
    uint8_t* data; 
    struct semaphore sem;
    struct cdev cdev;
};


extern int slank_major;

int slank_p_init(dev_t dev);
void slank_p_cleanup(void);


#endif /* _SLANK_H_ */
