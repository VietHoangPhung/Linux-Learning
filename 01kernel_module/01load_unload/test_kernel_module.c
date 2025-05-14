#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init test_kernel_module_init(void) {
    printk(KERN_INFO "Init\n");
    return 0;
}

static void __exit test_kernel_module_exit(void) {
    printk(KERN_INFO "Exit\n");
}

module_init(test_kernel_module_init);
module_exit(test_kernel_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HoangNgu");
MODULE_DESCRIPTION("kernel to print msg after loanding / unloading");