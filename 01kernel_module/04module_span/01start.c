#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

int start_init(void);

int __init start_init(void) {
    printk(KERN_INFO "full_module loaded, implement in 01start.c\n");
    return 0;
}

module_init(start_init);