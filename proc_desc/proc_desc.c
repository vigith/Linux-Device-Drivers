
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */

#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */

#include "proc_desc.h"

MODULE_AUTHOR("Vigith Maurice");
MODULE_LICENSE("Dual BSD/GPL");

int pd_major = PD_MAJOR;
int pd_minor = 0;
int pd_nr    = PD_NR;

struct pd_dev *pd_device;


/*
 * 'open' system call
 */
int pd_open(struct inode *inode, struct file *filp) {
  struct pd_dev *dev;		/* our device, which contains 'cdev' */


  /* takes a pointer to a field of type container_field, within a  structure of 
     type container_type, and returns a pointer to the containing structure */
  dev = container_of(inode->i_cdev, struct pd_dev, cdev);
  /* for easier access, else we will have to call container_of everytime */
  filp->private_data = dev;

  /* if file is opened for write, clean the data */
  if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
    
    /* acquire a lock */
    if (down_interruptible(&dev->sem)) /* return error, if we can't */
      return -ERESTARTSYS;

    kfree(dev->pd);	/* remove the struct allocated */

    up(&dev->sem);   /* release the lock */
  }

  return 0;
}

/* 
 * Read from the device
 */
ssize_t pd_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
  struct pd_dev *dev = filp->private_data; /* we had stored 'dev' in private_data, else use container_of */
  printk(KERN_INFO "Pid is [%d]\n", dev->pd->pid);
  return 1;
}
                

/* 
 * setting up the file operations
 */
struct file_operations pd_fops = {
  .owner = THIS_MODULE,
  .open  = pd_open,
  .read  = pd_read,
};


/* 
 * Device Setup for each individual device 
 */
int pd_dev_setup(struct pd_dev *dev, int nr) {
  int err;
  dev_t devno = MKDEV(pd_major, pd_minor + nr);
  
  /* device registration, non-static for device specific registration */
  cdev_init(&dev->cdev, &pd_fops);
  dev->cdev.ops = &pd_fops;
  dev->cdev.owner = THIS_MODULE;

  /* tells the kernel about the registration */
  err = cdev_add (&dev->cdev, devno, 1);

  if (err) {
    printk(KERN_WARNING "Error during setting up 'proc_desc' device no [%d]\n", nr);
    return err;
  }

  /* create the 'pd' space */
  dev->pd = kmalloc(sizeof(struct pd_data) * 1, GFP_KERNEL);

  /* initialize the PID to -1, else if read is first we should say PID is not valid */
  dev->pd->pid = 1;

  return 0;
}

/*
 * Initialization Module Code, called during module insertion
 */
static int __init pd_init_module(void) {
  int result = -1;
  int i;
  dev_t dev  = 0;
  
  /* register the character device */
  result = alloc_chrdev_region(&dev, pd_minor, pd_nr, "proc_desc");
  if (result < 0) {
    pd_major = MAJOR(dev);
    printk(KERN_WARNING "proc_desc: can't get major %d\n", pd_major);
    return result;
  }

  pd_major = MAJOR(dev);

  /* allocate the devices */
  /* This is the flag to use in process context code when it is safe to sleep. The 
     kernel will do whatever it has to do to obtain the memory requested by the caller. */     
  pd_device = kmalloc(pd_nr * sizeof(struct pd_dev), GFP_KERNEL);


  /* setup each device */
  for (i=0; i<pd_nr; i++) {
    init_MUTEX(&pd_device[i].sem);
    pd_dev_setup(&pd_device[i], i); /* TODO: catch error and do good cleanup */
  }

  printk(KERN_INFO "Inserted Module 'pd_dev' [%d]\n", MAJOR(dev));
  return result;
}

/*
 * Exit Module Code, called during module removed
 */
static void __exit pd_exit_module(void) {
  dev_t devno = MKDEV(pd_major, pd_minor);
  int i;

  /* free the memory we acquired for each device */
  for (i=0; i<pd_nr; i++) {
    /* free the pd_data first, else pointer to be it will be lost */
    kfree(pd_device[i].pd);
    /* unregister the character device */
    cdev_del(&pd_device[i].cdev);
  }

  /* clean the device sctructure */
  kfree(pd_device);

  unregister_chrdev_region(devno, pd_nr);
  printk(KERN_INFO "Removed Module 'pd_dev' [%d]\n", MAJOR(devno));
}

module_init(pd_init_module);
module_exit(pd_exit_module);
