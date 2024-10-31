#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>        // For character device registration
#include <linux/cdev.h>      // For cdev structure
#include <linux/uaccess.h>   // For copy_to_user, copy_from_user
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#define BUFFER_SIZE 256

int kernel_version[3];
module_param_array(kernel_version, int, NULL, S_IRUSR | S_IWUSR);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev os_cdev;
uint8_t *device_buffer;

static int __init os_driver_init(void);
static void __exit os_driver_exit(void);
static int os_device_open(struct inode *inode, struct file *file);
static int os_device_release(struct inode *inode, struct file *file);
static ssize_t os_device_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t os_device_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = os_device_read,
    .write = os_device_write,
    .open = os_device_open,
    .release = os_device_release,
};

// Called when the device file is opened
static int os_device_open(struct inode *inode, struct file *file)
{
    pr_info("Driver Open Function Called...!!!\n");
    return 0;
}

// Called when the device file is closed
static int os_device_release(struct inode *inode, struct file *file)
{
    pr_info("Driver Release Function Called...!!!\n");
    return 0;
}

// Called when the device file is read
static ssize_t os_device_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    int bytes_to_read;

    pr_info("Driver Read Function Called...!!!\n");

    if (*off >= BUFFER_SIZE)
        return 0; // EOF: no more data to read
    if(len < BUFFER_SIZE - *off)
        bytes_to_read = len;
    else
        bytes_to_read= BUFFER_SIZE - *off;

    if (copy_to_user(buf, device_buffer + *off, bytes_to_read)) {
        pr_err("Data Read: Error copying to user\n");
        return -EFAULT;
    }

    *off += bytes_to_read; // Update the file offset
    return bytes_to_read;
}

// Called when the device file is written
static ssize_t os_device_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    int bytes_to_write;

    pr_info("Driver Write Function Called...!!!\n");

    if(len < BUFFER_SIZE)
        bytes_to_write = len;
    else
        bytes_to_write = BUFFER_SIZE;

    if (copy_from_user(device_buffer, buf, bytes_to_write)) {
        pr_err("Data Write: Error copying from user\n");
        return -EFAULT;
    }

    return bytes_to_write;
}

static int __init os_driver_init(void)
{
    int target_version = KERNEL_VERSION(kernel_version[0], kernel_version[1], kernel_version[2]);
    if (target_version != LINUX_VERSION_CODE) {
        printk(KERN_ERR "Kernel version mismatch. Module not loaded.\n");
        return -EINVAL;
    }

    printk(KERN_INFO "Kernel version matches. Proceeding with module insertion.\n");

    // Allocate major and minor numbers
    if (alloc_chrdev_region(&dev, 0, 1, "OS_A2") < 0) {
        printk(KERN_INFO "Failed to allocate inode number\n");
        return -1;
    }

    pr_info("MajorNo=%d\tMinorNo=%d\n", MAJOR(dev), MINOR(dev));

    // Initialize cdev structure
    cdev_init(&os_cdev, &fops);

    // Add character device to the system
    if (cdev_add(&os_cdev, dev, 1) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    // Create struct class
    dev_class = class_create(THIS_MODULE, "os_driver_class");
    if (IS_ERR(dev_class)) {
        pr_err("Cannot create the struct class for device\n");
        goto r_class;
    }

    // Create device
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "os_driver_device"))) {
        pr_err("Cannot create the device\n");
        goto r_device;
    }

    // Allocate buffer memory
    device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!device_buffer) {
        pr_err("Cannot allocate memory in kernel\n");
        goto r_device;
    }

    pr_info("Kernel Module Successfully Inserted..\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit os_driver_exit(void)
{
    kfree(device_buffer);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&os_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Kernel Module Removed Successfully...\n");
}

module_init(os_driver_init);
module_exit(os_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nandana");
MODULE_DESCRIPTION("Character device driver for OS assignment 2");
