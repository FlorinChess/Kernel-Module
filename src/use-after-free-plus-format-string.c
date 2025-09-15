#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "memop_device"
#define BUF_SIZE 128

static int major;
static char* kernel_buffer;
static size_t data_size = 0;

static int dev_open(struct inode*nodep, struct file* filep);
static int dev_release(struct inode*nodep, struct file* filep);
static int dev_clear(struct inode*nodep, struct file* filep);
static ssize_t dev_write(struct file* filep, const char __user* buffer, size_t len, loff_t* offset);
static ssize_t dev_read(struct file* filep, char __user* buffer, size_t len, loff_t* offset);

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .write = dev_write,
    .read = dev_read,
};

static int dev_clear(struct inode* inodep, struct file *filep)
{
  printk(KERN_INFO "[memop] Clearing kernel buffer...\n");
  kfree(kernel_buffer);
}

static int dev_open(struct inode* inodep, struct file *filep)
{
    printk(KERN_INFO "[memop] Device opened.\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "[memop] Device closed.\n");
    return 0;
}

static ssize_t dev_write(struct file* filep, const char __user* buffer, size_t len, loff_t* offset)
{
    long copied;
    size_t to_copy;

    to_copy = (len > BUF_SIZE - 1) ? BUF_SIZE - 1 : len;

    copied = strncpy_from_user(kernel_buffer, buffer, to_copy);
    if (copied < 0) {
        printk(KERN_ERR "[memop] Failed to copy from user.\n");
        return -EFAULT;
    }

    kernel_buffer[to_copy] = '\0';
    data_size = to_copy;

    printk(KERN_INFO "[memop] Received string: %s\n", kernel_buffer);

    /* Reverse the string in-place */
    for (size_t i = 0; i < data_size / 2; i++) {
        char tmp = kernel_buffer[i];
        kernel_buffer[i] = kernel_buffer[data_size - i - 1];
        kernel_buffer[data_size - i - 1] = tmp;
    }
    
    printk(KERN_INFO "[memop] String reversed: ");
    printk(kernel_buffer);

    return to_copy;
}

static ssize_t dev_read(struct file* filep, char __user* buffer, size_t len, loff_t* offset)
{
    size_t bytes_to_read;

    if (*offset >= data_size)
        return 0;  // EOF

    bytes_to_read = (len > data_size - *offset) ? data_size - *offset : len;

    if (copy_to_user(buffer, kernel_buffer + *offset, bytes_to_read))
        return -EFAULT;

    *offset += bytes_to_read;
    return bytes_to_read;
}

static int __init memop_init(void)
{
    printk(KERN_INFO "[memop] Initializing module.\n");

    kernel_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        printk(KERN_ERR "[memop] Failed to allocate buffer.\n");
        return -ENOMEM;
    }

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "[memop] Failed to register device.\n");
        kfree(kernel_buffer);
        return major;
    }

    printk(KERN_INFO "[memop] Module loaded. Major number: %d\n", major);
    return 0;
}

static void __exit memop_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    kfree(kernel_buffer);
    printk(KERN_INFO "[memop] Module unloaded.\n");
}

module_init(memop_init);
module_exit(memop_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Florin Zamfir");
MODULE_DESCRIPTION("Use-after-free + string format vulnerability demo.");
MODULE_VERSION("1.0");
