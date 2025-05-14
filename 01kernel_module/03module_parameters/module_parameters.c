#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HoangNgu");

static int arg = 10;
module_param(arg, int, S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(arg, "arg an integer value");

int module_parameters_init(void);
void module_parameters_exit(void);




int __init module_parameters_init(void) {
    printk(KERN_INFO "module_parameters loaded\n");
    printk(KERN_INFO "arg: %d\n", arg);
    return 0;
}

void __exit module_parameters_exit(void) {
    printk(KERN_INFO "module_parameters unloaded\n");
}

module_init(module_parameters_init);
module_exit(module_parameters_exit);