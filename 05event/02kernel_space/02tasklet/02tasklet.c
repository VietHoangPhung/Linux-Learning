/**
 * main thread simply print log every 2 seconds
 * 
 * events caused by a timer
 * use tasklet to handle timer events, allow other interrupt to interrupt the timer event handler
 * timer events will sleep 1 sec to simulate work
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/interrupt.h>


MODULE_LICENSE("GPL"); 

static struct timer_list timer;


// timer callback
static void timer_callback(struct timer_list*);

// timer event handler
void timer_event_handler(struct tasklet_struct*);

DECLARE_TASKLET(tasklet, timer_event_handler);

/** init */
static int __init my_module_init(void) {
    // set up the timer
    timer_setup(&timer, timer_callback, 0);

    // trigger event every 1 sec
    mod_timer(&timer, jiffies + msecs_to_jiffies(5000));
    return 0;
}

/** exit */
static void __exit my_module_exit(void) {
    del_timer_sync(&timer);
    tasklet_kill(&tasklet);
    printk(KERN_INFO "02tasklet unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

// timer callback
static void timer_callback(struct timer_list*) {
    printk(KERN_INFO "timer event triggered, timer event handler executed by tasklet\n");

    // schedule tasklet
    tasklet_schedule(&tasklet);

    // set the timer again
    mod_timer(&timer, jiffies + msecs_to_jiffies(5000));
}

// timer event handler
void timer_event_handler(struct tasklet_struct* t) {
    unsigned int count = 2000000001;

    while(count--) {
        if(count % 200000000 == 0)
            printk(KERN_INFO "tasklet is still busy, count = %u\n", count);
    }
}
