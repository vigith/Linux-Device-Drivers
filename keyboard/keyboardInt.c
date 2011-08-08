#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>

/* This function services keyboard interrupts */
irq_handler_t irq_handler (int irq, void *dev_id, struct pt_regs *regs) {
  static unsigned char scancode;
	
  /*
    Read keyboard status
  */
  scancode = inb (0x60);

  if ((scancode == 0x01) || (scancode == 0x81))
    {
      printk ("You pressed Esc !\n");
    } else {
    printk("You pressed [%c] [%d] [%s]\n", scancode, scancode, scancode & 0x80 ? "Released" : "Pressed");
  }

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


