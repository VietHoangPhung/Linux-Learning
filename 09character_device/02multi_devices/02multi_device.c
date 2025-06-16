/**
 * mutli device by a kernel module
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <linux/device/class.h>
#include <linux/cdev.h>

#define DEVICE_COUNT    4

#define BUFFER_SIZE     1024
#define CLASS_NAME      "vjp_class"
#define DEVICE_NAME     "vjp_device"

/** some permission macros */
#define READ_M       0b01    // readable
#define WRITE_M      0b10    // writable


/** some typedef */

/** device struct */
typedef struct {
    char* buffer[BUFFER_SIZE];   // 1024 bytes          device's internal buffer
    struct device* mdevice;      // ~ 1000 bytes        
    struct class* mclass;        // 104 bytes
    struct cdev mcdev;           // ~ 130 bytes
    unsigned int id;             // 4 bytes             device's id
    dev_t dev_num;               // 4 bytes             device number
    size_t data_size;
    u8 perms;                    // 1 bytes             device file's permision     simply read/write for 2 first lsb: 1 = allowed; 0 = peohibited. first lsb = read perm, 2nd lsb = write perm
} mdev_t;
/** 
struct platform_driver {
    dev_t;
    cdev;
    class;
} **/

/** functions prototypes */

// #define IS_READABLE(mdev_t)    ((mdev_t).perms & READ_MASK) 
// #define IS_WRITABLE(mdev_t)    ((mdev_t).perms & WRITE_MASK) 
static int dev_open(struct inode* inode, struct file* file);
static int dev_release(struct inode* inode, struct file* file);
static ssize_t dev_read(struct file* file, char __user* buf, size_t count, loff_t* offset);
static ssize_t dev_write(struct file* file, const char __user* buf, size_t count, loff_t* offset);
static inline u8 is_readable(mdev_t*);
static inline u8 is_writable(mdev_t*);
static inline void add_perms(mdev_t*, u8);
/** global vars */
/** file operations */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static mdev_t* devices;
static dev_t base_dev_num;


/** init */
static int __init multi_device_init(void) {
    // alloc 4 device numbers with base_dev_num, minor from 0 to 3
    alloc_chrdev_region(&base_dev_num, 0, 16, DEVICE_NAME);

    // alloc array to store devices
    devices = kzalloc(16 * sizeof(mdev_t), GFP_KERNEL);

    // create same class for all devices
    struct class* vjp_class = class_create(CLASS_NAME);

    //cdev_init()

    // create devices
    for(int i = 0; i < DEVICE_COUNT; i++) {
        devices[i].id = i;      // id

        /** each dev_num by base_dev_num + i, remain the same major number with specific minor */
        devices[i].dev_num = MKDEV(MAJOR(base_dev_num), MINOR(base_dev_num) + i);

        // assign operations
        cdev_init(&devices[i].mcdev, &fops);
        cdev_add(&devices[i].mcdev, devices[i].dev_num, 1);

        // class
        devices[i].mclass = vjp_class;

        // create device
        devices[i].mdevice = device_create(devices[i].mclass, NULL, devices[i].dev_num, NULL, "%s%d", DEVICE_NAME, i);

        add_perms(&devices[i], i);      // 00 01 10 11 // NO RO WO RW
        printk("mdev: %s%d initialized, MAJOR = %d, MINOR = %d\n", DEVICE_NAME, i, MAJOR(devices[i].dev_num), MINOR(devices[i].dev_num));
    }
    devices[5].mdevice = device_create(devices[5].mclass, NULL, devices[5].dev_num, NULL, "%s%d", DEVICE_NAME, 5);

    return 0;
}

/** exit */
static void __exit multi_device_exit(void) {
    for(int i = 0; i < DEVICE_COUNT; i++) {
        // destroy device
        device_destroy(devices[i].mclass, devices[i].dev_num);

        // remove cdev from kernel
        cdev_del(&devices[i].mcdev);
    }

    // destroy class
    class_destroy(devices[0].mclass);

    // unregister major & minor numbers
    unregister_chrdev_region(base_dev_num, DEVICE_COUNT);


    // free the allocated spcae for devices
    kfree(devices);
    printk(KERN_INFO "removed all vjp devices\n");
}

/** definitions */
static int dev_open(struct inode* inode, struct file* file) {
    // I dont know how can it know which specific device file is opened
    // get cdev struct from inode
    struct cdev* cdev = inode->i_cdev;

    // get the mdev_t struct from the inode's cdev
    mdev_t *dev = container_of( cdev,              // what got from inode's cdev
                                mdev_t,            // the parent structure
                                mcdev);            // the member in the parent structure 

    // assign pointer to the opened device to the file->private for other operations
    file->private_data = (void*)dev;

    printk(KERN_INFO "%s%d opened", DEVICE_NAME, dev->id);
    return 0;
}

static int dev_release(struct inode* inode, struct file* file) {
    // here what I got from dev_open, a pointer to this device's struct
    mdev_t* dev = (mdev_t*)file->private_data;
    printk(KERN_INFO "%s%d released\n", DEVICE_NAME, dev->id);
    return 0;
}

static ssize_t dev_read(struct file* file, char __user* buf, size_t count, loff_t* offset) {
    mdev_t* dev = (mdev_t*)file->private_data;

    if(!is_readable(dev))
        return -EPERM;

    size_t bytes_to_read;


    // read beyond the available data
    if(*offset >= dev->data_size)
        return 0;

    bytes_to_read = dev->data_size - *offset;
    if(count > bytes_to_read)
        count = bytes_to_read;

    copy_to_user(buf, dev->buffer + *offset, count);

    *offset += count;

    printk(KERN_INFO "%s%d: read %zu bytes\n", DEVICE_NAME, dev->id, count);
    return count;
}


static ssize_t dev_write(struct file* file, const char __user* buf, size_t count, loff_t* offset) {
    mdev_t* dev = (mdev_t*)file->private_data;

    if(!is_writable(dev)) {
        return -EPERM;
    } 
    size_t space_left = BUFFER_SIZE - *offset;

    if(count > space_left)
        count = space_left;

    copy_from_user(dev->buffer + *offset, buf, count);

    *offset += count;
    if(*offset > dev->data_size)
        dev->data_size = *offset;

    printk(KERN_INFO "%s%d: wrote %zu bytes to device\n",DEVICE_NAME, dev->id, count);
    return count;
}

/** some helper functions */
static inline u8 is_writable(mdev_t* dev) {
    return (dev->perms & WRITE_M);
}

static inline u8 is_readable(mdev_t* dev) {
    return (dev->perms & READ_M);
}

inline void add_perms(mdev_t* dev, u8 perms) {
    dev->perms = perms;
}

module_init(multi_device_init);
module_exit(multi_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hoangu");
MODULE_DESCRIPTION("Multi-device char driver using struct");