#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>        // For character device registration
#include <linux/cdev.h>      // For cdev structure
#include <linux/uaccess.h>   // For copy_to_user, copy_from_user

/*****************************************************************************************
 *  file name : driver.c
 *  details:    OS assignment 2 Character device driver
 *  author:     Nandana Anand
 *  roll:       B220424CS
 * *************************************************************************************** **/


static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("Nandana");

MODULE_DESCRIPTION("This driver is created as part of OS theory Assignment 2");