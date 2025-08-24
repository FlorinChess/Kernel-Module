#include <linux/module.h>   /* Needed by all modules  */
#include <linux/kernel.h>   /* Needed for KERN_INFO   */
#include <linux/init.h>     /* Needed for the macros  */
#include <linux/list.h>     /* Needed for linked list */
#include <linux/types.h>    /* Needed for list macros */
#include <linux/slab.h>     /* Needed for Kernel */
#include <linux/uaccess.h>

#define READ_SIZE 16

void print_hello(char* username) {
  printk(KERN_INFO "Hello %s !\n", username);
  username = NULL;
}

static int __init init_hello(void)
{
  char username_buffer[READ_SIZE];
  strncpy_from_user(username_buffer, (char*)0x0000005000000, READ_SIZE);

  print_hello(username_buffer);

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
