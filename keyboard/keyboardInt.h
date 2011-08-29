
#ifndef _KEYBOARD_STATS_
#define _KEYBOARD_STATS_

#ifndef KYBRD_MAJOR
#define K_MAJOR 0		/* dynamic major number */
#endif

#ifndef KYBRD_STR_SIZE
#define K_STR_SIZE 1024*4*4     /* data size that is to be printed out */
#endif

/* our device structure */
struct keyboard_stats_dev {
  char *stat_str;		/* string that would be shown to user */
  struct semaphore sem;		/* mutual exclusion semaphore  */
  struct cdev cdev;		/* Char device structure       */
};

/* scan code */
static unsigned char scancode;

#endif	/* _KEYBOARD_STATS_ */
