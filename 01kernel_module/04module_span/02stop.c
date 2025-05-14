#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

void stop_init(void);

void __exit stop_init(void) {
    printk(KERN_INFO "full_module unloaded, implement in 02stop.c\n");
}

module_exit(stop_init);