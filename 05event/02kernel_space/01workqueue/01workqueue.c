/**
 * main thread simply print log every 2 seconds
 * 
 * events caused by a timer
 * use workqueue to handle timer events
 * timer events will sleep 1 sec to simulate work
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL"); 

static struct timer_list timer;
static struct workqueue_struct* workqueue;
static struct work_struct work;

static struct task_struct* main_thread;

// main thread function
static int main_thread_func(void*);

// timer callback
static void timer_callback(struct timer_list*);

// timer event handler
static void timer_event_handler(struct work_struct*);

/** init */
static int __init my_module_init(void) {
    workqueue = create_singlethread_workqueue("my_workqueue");

    // assigned work vs its handler
    INIT_WORK(&work, timer_event_handler);

    // set up the timer
    timer_setup(&timer, timer_callback, 0);

    // trigger event every 1 sec
    mod_timer(&timer, jiffies + msecs_to_jiffies(1000));

    // create main thread
    main_thread = kthread_create(main_thread_func, NULL, "main_thread");

    // start main thread
    wake_up_process(main_thread);
    return 0;
}

/** exit */
static void __exit my_module_exit(void) {
    kthread_stop(main_thread);
    del_timer_sync(&timer);
    destroy_workqueue(workqueue);
}

module_init(my_module_init);
module_exit(my_module_exit);

// main thread function
static int main_thread_func(void *data) {
    int count = 0;

    while(!kthread_should_stop()) {     // should never be without this shiet or this module are livin forever in your kernel
        count++;
        printk(KERN_INFO "main thread TID = %d, count = %d\n", main_thread->pid, count);
        msleep(3000);
    }

    printk(KERN_INFO "thread stopping\n");

    return 0;
}

// timer callback
static void timer_callback(struct timer_list*) {
    printk(KERN_INFO "timer event triggered, pushing work to workqueue\n");
    queue_work(workqueue, &work);

    // set the timer again
    mod_timer(&timer, jiffies + msecs_to_jiffies(2000));
}

// timer event handler
static void timer_event_handler(struct work_struct*) {
    static int count = 0;

    count++;
    printk(KERN_INFO "from workqueue, count = %d. Sleeping for 1 sec\n", count);
    msleep(1000);
}
