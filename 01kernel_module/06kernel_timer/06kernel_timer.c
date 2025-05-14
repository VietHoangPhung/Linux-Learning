#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("to test kernel timer module");

struct timer_list my_timer;


static int __init timer_module_init(void);

static void __exit timer_module_exit(void);

static void timer_callback(struct timer_list* timer);

static int __init timer_module_init(void) {
    printk(KERN_INFO "my timer module loaded successfully\n");
    timer_setup(&my_timer, timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
    return 0;
}

static void __exit timer_module_exit(void) {
    if(timer_pending(&my_timer)) {
        del_timer(&my_timer);
    }

    printk(KERN_INFO "timer deleted\n");
    printk(KERN_INFO "my timer module unloaded\n");
}

static void timer_callback(struct timer_list* timer) {
    static int count = 0;
    count++;
    printk(KERN_INFO "loop count: %d\n", count);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
}

module_init(timer_module_init);
module_exit(timer_module_exit);