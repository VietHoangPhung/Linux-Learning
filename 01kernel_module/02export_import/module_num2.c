#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("use function included in module_num1");

extern int add(int, int);
extern int sub(int, int);

int module_num2_init(void);
void module_num2_exit(void);

int __init module_num2_init() {
    printk(KERN_INFO "module_num2 loaded successfully\n");
    printk(KERN_INFO "this shit needs module_num1 loaded\n");
    printk(KERN_INFO "module_num1 loaded successfully\n4 - 5 + 3 = %d\n", add(sub(4, 5), 3));
    return 0;
}

void __exit module_num2_exit() {
    printk(KERN_INFO "module_num2 unloaded\n");
}

module_init(module_num2_init);
module_exit(module_num2_exit);
