// File: mainthread_workqueue_example.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

struct timer_list my_timer;
struct workqueue_struct *my_wq;
struct work_struct my_work;

struct task_struct *thread;

// Heavy work handler (executed in process context)
void my_work_handler(struct work_struct *work)
{
    static int count = 0;

    count++;
    pr_info("my_work_handler: heavy work %d\n", count);
}

void my_timer_callback(struct timer_list *timer)
{
    pr_info("my_timer_callback: event triggered!\n");

    // Schedule heavy work
    queue_work(my_wq, &my_work);

    // Rearm the timer to expire again in 1-second
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000)); 
}

// Main kernel thread function
int thread_fn(void *data)
{
    int count = 0;

    while (!kthread_should_stop()) {
        count++;
        pr_info("thread_fn: alive %d\n", count);
        msleep(3000);
    }

    pr_info("thread_fn: stopping\n");

    return 0;
}

static int __init my_module_init(void)
{
    pr_info("Init mainthread_workqueue_example\n");

    // Create a singlethreaded workqueue
    my_wq = create_singlethread_workqueue("my_wq");

    if (!my_wq) {
        pr_err("Unable to create workqueue\n");

        return -ENOMEM;
    }

    // Initialize the work
    INIT_WORK(&my_work, my_work_handler);

    // Initialize and start the timer
    timer_setup(&my_timer, my_timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

    // Create main kernel thread
    thread = kthread_run(thread_fn, NULL, "my_thread");

    if (IS_ERR(thread)) {
        pr_err("Unable to create kernel thread\n");

        destroy_workqueue(my_wq);
        del_timer_sync(&my_timer);
        return PTR_ERR(thread);
    }

    return 0;
}

static void __exit my_module_exit(void)
{
    pr_info("Exit mainthread_workqueue_example\n");

    kthread_stop(thread);
    del_timer_sync(&my_timer);
    destroy_workqueue(my_wq);
}

module_init(my_module_init);
module_exit(my_module_exit);
