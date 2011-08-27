#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/string.h>

#include "keyboardInt.h"

/* Spin lock */
DEFINE_SPINLOCK(mr_lock);

/* Tasklets for BH */
void keyboard_tasklet_bh(unsigned long);
// PROTO: DECLARE_TASKLET(name, function, data); 
DECLARE_TASKLET(keyboard_tasklet, keyboard_tasklet_bh, 0);

void keyboard_tasklet_bh(unsigned long hits) {
  int binary;
  unsigned char scode;		/* tmp scancode */
  char *bin, *tmp;

  bin= kmalloc(sizeof(unsigned char) * 1, GFP_ATOMIC); /* pid is read a a string */
  tmp = kmalloc(sizeof(unsigned char) * 1, GFP_ATOMIC); /* needed for simple_strtol */

  /* save the scan code */
  spin_lock(&mr_lock);
  scode = scancode;
  spin_unlock(&mr_lock);

  /* since no TASKLETs run together, i don't have to worry about locks here */
  if ((scancode == 0x01) || (scancode == 0x81))
    {
      printk ("You pressed Esc !\n");
    } else {
    sprintf(bin, "%d", scancode);
    binary = simple_strtol(bin, &tmp, 10);
    if (scancode & 0x80) {
      binary -= 128;		/* key release - 128 is key press */
      keyboard_stats[binary].count_r += 1;
    } else
      keyboard_stats[binary].count_p += 1;
    printk("You pressed [%x] [%d] [%s] (%d %d)\n", scancode, scancode, scancode & 0x80 ? "Released" : "Pressed", keyboard_stats[binary].count_r, keyboard_stats[binary].count_p);
    
  }
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

/* Initialize the module and Register the IRQ handler */
static int __init keybrd_int_register(void)
{
  int result;
  /* Request IRQ 1, the keyboard IRQ */    
  result = request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "test_keyboard_irq_handler", (void *)(irq_handler));

  if (result)
    printk(KERN_INFO "can't get shared interrupt for keyboard\n");
  
  return result;
}

/* Remove the interrupt handler */
static void __exit keybrd_int_unregister(void) {
  free_irq(1, (void *)(irq_handler)); /* i can't pass NULL, this is a shared interrupt handler! */
}

MODULE_LICENSE ("GPL");
MODULE_AUTHOR("Vigith Maurice");
module_init(keybrd_int_register);
module_exit(keybrd_int_unregister);


