#include <linux/module.h>   /* Needed by all modules  */
#include <linux/kernel.h>   /* Needed for KERN_INFO   */
#include <linux/init.h>     /* Needed for the macros  */
#include <linux/list.h>     /* Needed for linked list */
#include <linux/types.h>    /* Needed for list macros */
#include <linux/slab.h>     /* Needed for Kernel */
#include <linux/uaccess.h>

#define SIZE 16
#define MESSAGE_ADDRESS (char*)0x0000005004000
#define START_COMMAND "start"
#define CANCEL_COMMAND "cancel"
#define SEND_COMMAND "send"

static int read_command(const char* command_address)
{
  char* command_buffer = kmalloc(SIZE, GFP_KERNEL);
  strncpy_from_user(command_buffer, command_address, SIZE);

  char* message_buffer = NULL;

  if (strncmp(command_buffer, START_COMMAND, sizeof(START_COMMAND))) 
  {
    printk("Start command recieved! Reading message from userspace...\n");
    message_buffer = kmalloc(SIZE, GFP_KERNEL);
    strncpy_from_user(command_buffer, MESSAGE_ADDRESS, SIZE);

  } 
  
  if (strncmp(command_buffer, SEND_COMMAND, sizeof(CANCEL_COMMAND))) 
  {
    printk("Cancel command received! Canceling operation!\n");
    kfree(message_buffer);      
  } 
  
  if (strncmp(command_buffer, SEND_COMMAND, sizeof(SEND_COMMAND))) 
  {
    printk("Send command received! Sending the following message: %s\n", message_buffer);
  } 
  else 
  {
    printk("Unknown command! \n");
  }

  kfree(command_buffer);

  return 0;
}

static int __init init_command_reader(void)
{
  printk(KERN_INFO "Module intilized!\n");
  return 0;
}

static void __exit cleanup_command_reader(void) {
  printk(KERN_INFO "Cleanup complete!\n");
}

module_init(init_command_reader);
module_exit(cleanup_command_reader);

MODULE_AUTHOR("Florin Zamfir");
MODULE_DESCRIPTION("Use-after-free vulnerability demo.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
