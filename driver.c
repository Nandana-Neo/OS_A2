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

int kernel_version[3];
module_param_array(kernel_version,int, NULL, S_IRUSR|S_IWUSR);


dev_t dev=0;

static int __init driver_init(void)
{
    //Check kernel version
    if (kernel_version[0] != LINUX_VERSION_CODE >> 16 ||       // Major version
        kernel_version[1] != (LINUX_VERSION_CODE >> 8 & 0xFF) || // Minor version
        kernel_version[2] != (LINUX_VERSION_CODE & 0xFF)) {      // Patch version
        printk(KERN_ERR "Kernel version mismatch. Module not loaded.\n");
        return -EINVAL;  // Return an error if the version does not match
    }

    printk(KERN_INFO "Kernel version matches. Proceeding with module insertion.\n");

    //Printing major -- dynamically allocated
    if(alloc_chrdev_region(&dev,0,1,"OS_A2") < 0){
        printk(KERN_INFO "Failed to allocate inode number\n");
        return -1;
    }
    return 0;

    printf(KERN_INFO "MajorNo=%d\tMinor number=%d\n",MAJOR(dev),MINOR(dev));
}

static void __exit driver_exit(void)
{
    printk(KERN_INFO "Goodbye world\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("Dual BSD/GPL");

MODULE_AUTHOR("Nandana");

MODULE_DESCRIPTION("This driver is created as part of OS theory Assignment 2");