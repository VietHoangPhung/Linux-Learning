#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

static char* levels[8] = {KERN_EMERG,  KERN_ALERT,   KERN_CRIT,   KERN_ERR,   KERN_WARNING,   KERN_NOTICE,   KERN_INFO,   KERN_DEBUG};
static char* hints[8] = {"KERN_EMERG", "KERN_ALERT", "KERN_CRIT", "KERN_ERR", "KERN_WARNING", "KERN_NOTICE", "KERN_INFO", "KERN_DEBUG"};

static int level = 6;
module_param(level, int, S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(level, "log level");

static int __init module_test_init(void);
static void __exit module_test_exit(void);

static int __init module_test_init(void) {
    printk(KERN_INFO "module_test_log loaded\n");

    printk("%s%s: log level %d\n", levels[level], hints[level], level);

    return 0;
}

static void __exit module_test_exit(void) {
    printk(KERN_INFO "module_test_log unloaded\n");
}

module_init(module_test_init);
module_exit(module_test_exit);
