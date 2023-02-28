#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

#define MODULE_NAME "lkm3"
#define COMM_LEN 16

MODULE_DESCRIPTION("LKM3");
MODULE_AUTHOR("Raja");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

/* This function is called when the module is loaded. */
static int __init lkm3_init(void)
{
    struct task_struct *task;
    char comm[COMM_LEN];
    
    pr_info("Module %s loaded...\n", MODULE_NAME);

    // https://stackoverflow.com/questions/8547332/efficient-way-to-find-task-struct-by-pid
    rcu_read_lock();
    task = pid_task(find_vpid(1), PIDTYPE_PID);

    if (task) {
        //https://stackoverflow.com/questions/59054053/linux-kernel-task-struct-void-stack
        strncpy(comm, task->comm, COMM_LEN);
        pr_info("TGID: %d | PID: %d | Name: %s | Kernel Stack Pointer: %llu (0x%px) \n", task->tgid, task->pid, comm, (unsigned long long)task->stack, task->stack);
    } else {
        pr_warn("No task found for pid 1/n");
    }
    rcu_read_unlock();

    return 0; /* success */
}

/* This function is called when the module is removed. */
static void __exit lkm3_exit(void)
{
    pr_info("Exited from module %s\n", MODULE_NAME);
}

module_init(lkm3_init);
module_exit(lkm3_exit);
