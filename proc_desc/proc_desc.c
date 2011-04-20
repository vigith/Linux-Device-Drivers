
#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* read, write ... etc */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>		/* cdev_init */

#include "proc_desc.h"

MODULE_AUTHOR("Vigith Maurice");
MODULE_LICENSE("Dual BSD/GPL");

int pd_major = PD_MAJOR;
int pd_minor = 0;
int pd_nr    = PD_NR;

struct pd_dev *pd_device;

struct file_operations pd_fops = {
  .owner = THIS_MODULE,
};

/* 
 * Device Setup for each device.
 */
int pd_dev_setup(struct pd_dev *dev, int nr) {
  int err;
  dev_t devno = MKDEV(pd_major, pd_minor + nr);
  
  /* device registration, non-static for device specific registration */
  cdev_init(&dev->cdev, &pd_fops);
  dev->cdev.owner = THIS_MODULE;

  /* tells the kernel about the registration */
  err = cdev_add (&dev->cdev, devno, 1);

  if (err) {
    printk(KERN_WARNING "Error during setting up 'proc_desc' device no [%d]\n", nr);
    return err;
  }
    
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

  /* allocate the devices */
  /* This is the flag to use in process context code when it is safe to sleep. The 
     kernel will do whatever it has to do to obtain the memory requested by the caller. */     
  pd_device = kmalloc(pd_nr * sizeof(struct pd_dev), GFP_KERNEL);


  /* setup each device */
  for (i=0; i<pd_nr; i++) {
    init_MUTEX(&pd_device[i].sem);
    pd_dev_setup(&pd_device[i], i); /* TODO: catch error and do good cleanup */
  }

  printk(KERN_INFO "Inserted Module 'pd_dev'\n");
  return result;
}

/*
 * Exit Module Code, called during module removed
 */
static void __exit pd_exit_module(void) {
  dev_t devno = MKDEV(pd_major, pd_minor);

  unregister_chrdev_region(devno, pd_nr);
  printk(KERN_INFO "Removed Module 'pd_dev'\n");
}

module_init(pd_init_module);
module_exit(pd_exit_module);
