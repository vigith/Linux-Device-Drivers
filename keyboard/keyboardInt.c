#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/string.h>
#include <linux/fs.h>		/* everything... */
#include <linux/cdev.h>		/* character device */

#include <linux/string.h>		/* strcat */
#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>        /* copy_*_user */

#include "keyboardInt.h"
#include "keyboard_mapping.h"

int major = K_MAJOR;
int minor = 0;
struct keyboard_stats_dev *k_dev;

/* Spin lock */
DEFINE_SPINLOCK(mr_lock);

/* Tasklets for BH */
void keyboard_tasklet_bh(unsigned long);
/* PROTO: DECLARE_TASKLET(name, function, data); */
DECLARE_TASKLET(keyboard_tasklet, keyboard_tasklet_bh, 0);

void keyboard_tasklet_bh(unsigned long hits) {
  int binary;
  unsigned char scode;		/* tmp scancode */
  char *bin, *tmp;

  /* GFP_ATOMIC is compulsory as this won't sleep */
  bin= kmalloc(sizeof(unsigned char) * 1, GFP_ATOMIC); /* pid is read a a string */
  tmp = kmalloc(sizeof(unsigned char) * 1, GFP_ATOMIC); /* needed for simple_strtol */

  /* save the scan code */
  spin_lock(&mr_lock);
  scode = scancode;
  spin_unlock(&mr_lock);

  /* since no TASKLETs run together, i don't have to worry about locks here */
  sprintf(bin, "%d", scancode);
  binary = simple_strtol(bin, &tmp, 10);
  
  /* save the key press/release stat */
  if (scancode & 0x80) {
    binary -= 128;		/* key release - 128 is key press */
    keyboard_stats[binary].count_r += 1;
  } else {
    keyboard_stats[binary].count_p += 1;
  }

  /* Please don't UNCOMMENT this block, in interrupt context printk can be blocked and put a system to a serious unstable state.   
     ('coz, interrupt state can't wake up becuase it has no process context and need_schedule() func won't be called (long story, read LINUX DEVICE DRIVERS 3rd ed).
     I tested in a VM box, where there are no other activities were taking place and rebooting was not at-all a problem :-)
  // printk(KERN_INFO "You pressed [%x] [%d] [%s] (%lu %lu)\n", scancode, scancode, scancode & 0x80 ? "Released" : "Pressed", keyboard_stats[binary].count_r, keyboard_stats[binary].count_p);
  */

  return;
}

/* This function services keyboard interrupts */
irq_handler_t irq_handler (int irq, void *dev_id, struct pt_regs *regs) {

  /*
    Read keyboard status.
    Obtain a spin lock and update scancode.
  */
  spin_lock(&mr_lock);
  scancode = inb (0x60);
  spin_unlock(&mr_lock);

  /* schedule the tasklet */
  tasklet_schedule(&keyboard_tasklet);

  return (irq_handler_t) IRQ_HANDLED;
}

/*
 * 'open' system call
 */
int k_dev_open(struct inode *inode, struct file *filp) {
  struct keyboard_stats_dev *dev;		/* our device (cdev), which contains 'cdev' */

  /* takes a pointer to a field of type container_field, within a  structure of 
     type container_type, and returns a pointer to the containing structure.
     This is for getting the parent keyboard_stats_dev struct from cdev which is
     encapsulated inside keyboard_stats_dev. (we don't need dev, but incase for future! */
  dev = container_of(inode->i_cdev, struct keyboard_stats_dev, cdev);
  /* for easier access, else we will have to call container_of everytime */
  filp->private_data = dev;

  /* we are planning only for O_RDONLY, so no special handler for O_WRONLY
     what will you write to the stats file? :-) */

  return 0;
}

/* 
 * Read from the device (write to userspace), 'read' syscall
 */
ssize_t k_dev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
  struct keyboard_stats_dev *dev = filp->private_data; /* we had stored 'dev' in private_data, else use container_of */
  ssize_t retval = 0;
  char tmp[256];		/* tmp space for sprintf to put the keyboard stats */
  int i = 0;
  char sep = '\t';
  
  /* acquire lock for concurrency */
  if(down_interruptible(&dev->sem))
    return -ERESTARTSYS;

  /* We expect all calls to be full requests, not partial ones (ie, not using offsets), yeah lazy Maurice, bad hack :-) */
  if (*f_pos != 0) {
    goto out;
  }

  dev->stat_str[0] = '\0';

  /* Populate the stats string! */
  for (i=1; i<=SCAN_CODES; i++) {
    sprintf(tmp, "%10s (%lu %lu)%c", keyboard_stats[i].str, keyboard_stats[i].count_r, keyboard_stats[i].count_p, sep);  
    strcat(dev->stat_str, tmp);
    if (i % 3 == 0) {
      sep = '\n';
      continue;
    }
    sep = '\t';
  }

  /* write pd_str to userspace */
  if (copy_to_user(buf, dev->stat_str, strlen(dev->stat_str))) {
    retval = -EFAULT;
    goto out;
  }

  *f_pos = strlen(dev->stat_str);
  retval = strlen(dev->stat_str);

 out:
  up(&dev->sem);
  return retval;
}


/* 
 * setting up the file operations for k_dev
 */
struct file_operations k_fops = {
  .owner = THIS_MODULE,
  .open  = k_dev_open,
  .read  = k_dev_read,
};

/* 
 * Device Setup for Keyboard Stats Device
 */
int k_dev_setup(struct keyboard_stats_dev *dev) {
  int err;
  dev_t devno = MKDEV(major, minor);
  
  /* device registration, could have done static registration too! */
  cdev_init(&dev->cdev, &k_fops);
  dev->cdev.ops = &k_fops;
  dev->cdev.owner = THIS_MODULE;

  /* tells the kernel about the registration */
  err = cdev_add(&dev->cdev, devno, 1);

  if (err) {
    printk(KERN_WARNING "Error during setting up 'keyboard_stats'\n");
    return err;
  }

  /* k_dev string space */
  dev->stat_str = kmalloc(sizeof(char) * K_STR_SIZE, GFP_KERNEL);
  memset(dev->stat_str, 0, sizeof(char) * K_STR_SIZE);

  return 0;
}


/* Initialize the module and Register the IRQ handler */
static int __init keybrd_int_register(void) {
  int result = -1;
  dev_t dev  = 0;
  
  /* register the character device 
  (dev_t *dev, firstminor, count, name); 
  */
  result = alloc_chrdev_region(&dev, minor, 1, "keyboard_stats");
  if (result < 0) {
    major = MAJOR(dev);
    printk(KERN_WARNING "keyboard_stats: can't get major %d\n", major);
    return result;
  }

  major = MAJOR(dev);

  /* allocate the devices */
  /* GFP_KERNEL is the flag to use in process context code when it is safe to sleep. The 
     kernel will do whatever it has to do to obtain the memory requested by the caller. */     
  k_dev = kmalloc(1 * sizeof(struct keyboard_stats_dev), GFP_KERNEL);


  /* setup each device */
  init_MUTEX(&(k_dev->sem));	/* addr of sem */
  result = k_dev_setup(k_dev); /* TODO: catch error and do good cleanup */
  if (result)
    return result;

  printk(KERN_INFO "Inserted Module 'keyboard_stats' [%d]\n", major);

  /* Request IRQ 1, the keyboard IRQ */    
  result = request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "keyboard_stats", (void *)(irq_handler));
  if (result)
    printk(KERN_INFO "can't get shared interrupt for keyboard\n");
  
  return result;
}

/* Remove the interrupt handler and the device file */
static void __exit keybrd_int_unregister(void) {
  dev_t devno = MKDEV(major, minor);

  free_irq(1, (void *)(irq_handler)); /* i can't pass NULL, this is a shared interrupt handler! */

  /* free the memory we acquired */
  /* clean the keyboard_stats_dev string space */
    kfree(k_dev->stat_str);
    cdev_del(&(k_dev->cdev));

  /* clean the device sctructure */
  kfree(k_dev);

  unregister_chrdev_region(devno, 1);
  printk(KERN_INFO "Removed Module 'keyboard_stats' [%d]\n", MAJOR(devno));
}

MODULE_LICENSE ("GPL");
MODULE_AUTHOR("Vigith Maurice");
module_init(keybrd_int_register);
module_exit(keybrd_int_unregister);


