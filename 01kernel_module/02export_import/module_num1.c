#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("include add, sub functions");

int add(int, int);
int sub(int, int);

EXPORT_SYMBOL(sub);
EXPORT_SYMBOL(add);

static int __init module_num1_init(void);
static void __exit module_num1_exit(void);


int add(int a, int b) {
    return (a + b);
}

int sub(int a, int b) {
    return (a - b);
}

static int __init module_num1_init() {
    printk(KERN_INFO "module_num1 loaded successfully\n");
    printk(KERN_INFO "2 + 3 - 2 = %d\n", sub(add(2, 3), 2));
    return 0;
}

static void __exit module_num1_exit() {
    printk(KERN_INFO "module_num1 unloaded\n");
}

module_init(module_num1_init);
module_exit(module_num1_exit);


