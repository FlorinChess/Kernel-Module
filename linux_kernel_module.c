#include <linux/module.h>   /* Needed by all modules  */
#include <linux/kernel.h>   /* Needed for KERN_INFO   */
#include <linux/init.h>     /* Needed for the macros  */
#include <linux/list.h>     /* Needed for linked list */
#include <linux/types.h>    /* Needed for list macros */
#include <linux/slab.h>     /* Needed for Kernel */

// Module Metadata


static int __init init_hello(void)
{
  printk(KERN_INFO "Hello world !\n");
  
  /*
  * A non 0 return means init_module failed; module can't be loaded.
  */
  return 0;
}

static void __exit cleanup_hello(void) {
  printk(KERN_INFO "Bye!\n");
}

module_init(init_hello);
module_exit(cleanup_hello);

MODULE_AUTHOR("Florin Zamfir");
MODULE_DESCRIPTION("Hello world driver");
MODULE_LICENSE("GPL");
