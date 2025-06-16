/** 
 * to do: permission (read only, write only)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/device/class.h>
#include <linux/cdev.h>

#define BUFFER_SIZE     1024

/** functions prototypes */
static int dev_open(struct inode* inode, struct file* file);
static int dev_release(struct inode* inode, struct file* file);
static ssize_t dev_read(struct file* file, char __user* buf, size_t count, loff_t* offset);
static ssize_t dev_write(struct file* file, const char __user* buf, size_t count, loff_t* offset);

/**  */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

// TODO combine cdev attributes (cdev, class, device, etc to a struct to manage)

static struct cdev my_cdev;
static struct class* my_class;
static struct device* my_device;
static dev_t dev_num;

static char device_buffer[BUFFER_SIZE];
static size_t data_size = 0;
/** init */
static int __init simple_device_init(void) {
    // register device 
    alloc_chrdev_region(&dev_num, 0, 1, "mydevice");    // dev_num, base minor, number, internal name for driver
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);         // dev_num = the first device, count = how many? 

    // create class and device
    my_class = class_create("my_class");
    my_device = device_create(my_class, NULL, dev_num, NULL, "mydevice");

    printk(KERN_INFO "mydevice: loaded simple device driver\n");
    printk(KERN_INFO "mydevice: major num = %d, minor num = %d\n", MAJOR(dev_num), MINOR(dev_num));
    
    return 0;
}


/** exit */
static void __exit simple_device_exit(void) {
    // destroyo device in /dev
    device_destroy(my_class, dev_num);

    // remove class from /sys/class
    class_destroy(my_class);

    // remove cdev from kernel
    cdev_del(&my_cdev);

    // unregister major and minor number
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "mydevice driver unloaded\n");
}

/** definitions */
static int dev_open(struct inode* inode, struct file* file) {
    printk(KERN_INFO "mydevice: opened\n");
    return 0;
}

static int dev_release(struct inode* inode, struct file* file) {
    printk(KERN_INFO "mydevice: released\n");
    return 0;
}

static ssize_t dev_read(struct file* file, char __user* buf, size_t count, loff_t* offset) {
    printk(KERN_INFO "mydevice/dev_read: \ncount = %zu\n%lld", count, *offset);
    size_t bytes_to_read;

    if(*offset >= data_size)
        return 0;

    bytes_to_read = data_size - *offset;
    if (count > bytes_to_read)
        count = bytes_to_read;

    copy_to_user(buf, device_buffer + *offset, count);


    *offset += count;

    printk(KERN_INFO "mydevice: read %zu bytes from device\n", count);
    return count;
}


static ssize_t dev_write(struct file* file, const char __user* buf, size_t count, loff_t* offset) {
    printk(KERN_INFO "mydevice/dev_write: \ncount = %zu\noffset = %lld", count, *offset);
    size_t space_left = BUFFER_SIZE - *offset;

    if(count > space_left)
        count = space_left;

    copy_from_user(device_buffer + *offset, buf, count);

    *offset += count;
    if(*offset > data_size)
        data_size = *offset;

    printk(KERN_INFO "mydevice: wrote %zu bytes to device\n", count);
    return count;
}

module_init(simple_device_init);
module_exit(simple_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hoangu");
MODULE_DESCRIPTION("Simple device");
