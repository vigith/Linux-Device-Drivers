
#include <linux/module.h>
#include <linux/string.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/sched.h>        /* current and everything */

#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>        /* copy_*_user */

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

    kfree(dev->pd->pd_str);	/* remove the pd_str allocated, we don't need it in O_WRONLY */

    up(&dev->sem);   /* release the lock */
  }

  return 0;
}

/* 
 * Read from the device (write to userspace)
 */
ssize_t pd_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
  struct pd_dev *dev = filp->private_data; /* we had stored 'dev' in private_data, else use container_of */
  ssize_t retval = 0;
  
  struct task_struct *task;
  short int flag = 0;

  /* acquire lock for concurrency */
  if(down_interruptible(&dev->sem))
    return -ERESTARTSYS;

  /* this is not a first time request, we already satisfied the callee earlier
   * (it would have been better to see the total size of struct and decide whether
   * this second request is legitimate, eg users can use read with len)
   */
  if (*f_pos != 0) {
    goto out;
  }

  /* if nothing is written to the device file, we can't fetch any task_struct */
  if (dev->pd->pid == -1) {
    /* write the data to pd_str */
    sprintf(dev->pd->pd_str, "Please write a pid to the device file\n");
  } else {
    for_each_process(task) {
      if (task->pid == dev->pd->pid) {
	flag = 1;
	sprintf(dev->pd->pd_str, "Requested Pid: [%d] Current Pid: [%d] Comm: [%s]\n",
		dev->pd->pid, current->pid, task->comm);
      }
    }
    if (!flag) 
      printk(KERN_ALERT "Pid [%d] doesn't seem to be in pid list!\n", dev->pd->pid);
  }

  /* write pd_str to userspace */
  if (copy_to_user(buf, dev->pd->pd_str, strlen(dev->pd->pd_str))) {
    retval = -EFAULT;
    goto out;
  }

  *f_pos = strlen(dev->pd->pd_str);
  retval = strlen(dev->pd->pd_str);

 out:
  up(&dev->sem);
  return retval;
}

/*
 * 'write' system call, when user writes to the device file
 */
ssize_t pd_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
  struct pd_dev *dev = filp->private_data; /* we had stored 'dev' in private_data, else use container_of */
  ssize_t retval = -ENOMEM;
  char *spid, *estr;
  pid_t ipid;

  spid = kmalloc(sizeof(pid_t) * 1, GFP_KERNEL); /* pid is read a a string */
  estr = kmalloc(sizeof(pid_t) * 1, GFP_KERNEL); /* needed for simple_strtol */
  memset(spid, 0, sizeof(pid_t) * 1);

  /* get a lock */
  if(down_interruptible(&dev->sem))
    return -ERESTARTSYS;	/* use -EINTR if there will be inconsistency if the call to function is made again */

  if (count > 100)
    count = 100;

  /* write the pid to spid */
  if(copy_from_user(spid, buf, count)) {
    retval = -EFAULT;
    goto out;
  }
  spid[count-1] = '\0';		/* i don't have to do this because of memset! */

  // string to long
  ipid = simple_strtol(spid, &estr, 10); /* 10 says, use decimal base */
  dev->pd->pid = ipid;

  /* update the file_pointer with new position */
  *f_pos = count;
  retval = count;

 out:
  up(&dev->sem);
  return retval;
}
                

/* 
 * setting up the file operations
 */
struct file_operations pd_fops = {
  .owner = THIS_MODULE,
  .open  = pd_open,
  .read  = pd_read,
  .write = pd_write,
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
  err = cdev_add(&dev->cdev, devno, 1);

  if (err) {
    printk(KERN_WARNING "Error during setting up 'proc_desc' device no [%d]\n", nr);
    return err;
  }

  /* create the 'pd' space */
  dev->pd = kmalloc(sizeof(struct pd_data) * 1, GFP_KERNEL);
  /* pd string space */
  dev->pd->pd_str = kmalloc(sizeof(char) * PD_STR_SIZE, GFP_KERNEL);
  memset(dev->pd->pd_str, 0, sizeof(char) * PD_STR_SIZE);

  /* initialize the PID to -1, else if read is first we should say PID is not valid */
  dev->pd->pid = -1;

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
  /* GFP_KERNEL is the flag to use in process context code when it is safe to sleep. The 
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
    /* clean the process descriptor string space */
    kfree(pd_device[i].pd->pd_str);
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
