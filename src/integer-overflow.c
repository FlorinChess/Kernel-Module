#include <linux/module.h>   /* Needed by all modules  */
#include <linux/kernel.h>   /* Needed for KERN_INFO   */
#include <linux/init.h>     /* Needed for the macros  */
#include <linux/list.h>     /* Needed for linked list */
#include <linux/types.h>    /* Needed for list macros */
#include <linux/slab.h>     /* Needed for Kernel */
#include <linux/uaccess.h>

#define KEY_LENGTH 19
#define KEY_ADDRESS (char*)0x0000005000000
#define SALT_LENGTH 24
#define SALT_ADDRESS (char*) 0x0000050000FF
#define CIPHERTEXT_LENGTH 4096
#define CIPHERTEXT_ADDRESS (char*)0x0000005001000

long key = 0;
long salt = 0;
char ciphertext[CIPHERTEXT_LENGTH];

void decrypt(char* ciphertext, long key, long salt) {
  char plaintext[CIPHERTEXT_LENGTH];
  
  for(int i = 0; i < CIPHERTEXT_LENGTH; i++) {

  }

  printk(KERN_INFO "Plaintext: %s\n", plaintext);
}

static int __init init_decryptor(void)
{
  char key_buffer[KEY_LENGTH];
  strncpy_from_user(key_buffer, KEY_ADDRESS, KEY_LENGTH);
  kstrtol(key_buffer, 10, &key);
  
  char salt_buffer[SALT_LENGTH];
  strncpy_from_user(salt_buffer, SALT_ADDRESS, SALT_LENGTH);
  kstrtol(salt_buffer, 10, &salt);


  strncpy_from_user(ciphertext, CIPHERTEXT_ADDRESS, CIPHERTEXT_LENGTH);

  decrypt(ciphertext, key, salt);

  return 0;
}

static void __exit cleanup_decryptor(void) {
  printk(KERN_INFO "Bye!\n");
}

module_init(init_decryptor);
module_exit(cleanup_decryptor);

MODULE_AUTHOR("Florin Zamfir");
MODULE_DESCRIPTION("Format string vulenrability demo.");
MODULE_LICENSE("GPL");
