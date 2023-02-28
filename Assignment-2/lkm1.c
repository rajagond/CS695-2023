#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

#define MODULE_NAME "lkm1"

MODULE_DESCRIPTION("LKM1");
MODULE_AUTHOR("Raja");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

// Ref: Kernel Programming Book, Ch 6
/* This function is called when the module is loaded. */
static int __init lkm1_init(void)
{
    struct task_struct *task;
    int process_count = 0;

    pr_info("Module %s inserted...\n", MODULE_NAME);
    pr_info("List of running and runnable processes:\n");
    rcu_read_lock();
    for_each_process(task) {
        if (task->__state == TASK_RUNNING) {
            process_count++;
            pr_info("TGID: %d | PID: %d | Name: %s\n", task->tgid, task->pid, task->comm);
        }
    }
    rcu_read_unlock();

    pr_info("Total # of running and runnable processes: %d\n", process_count);

    return 0;
}

/* This function is called when the module is removed. */
static void __exit lkm1_exit(void)
{
    pr_info("Exited from module %s\n", MODULE_NAME);
}

module_init(lkm1_init);
module_exit(lkm1_exit);
