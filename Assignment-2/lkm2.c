#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/mm_types.h>

#define MODULE_NAME "lkm2"
#define COMM_LEN 16

MODULE_DESCRIPTION("LKM2");
MODULE_AUTHOR("Raja");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

/* This function is called when the module is loaded. */
static int __init lkm2_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;
    unsigned long long max_heap_mem = 0;
    pid_t pid = 1;
    pid_t tgid = 1;
    char comm[COMM_LEN];
    unsigned long long heap_mem = 0;

    pr_info("Loaded module %s...\n", MODULE_NAME);
    rcu_read_lock();
    for_each_process(task) {
        mm = task->mm;
        if (mm) {
            down_read(&mm->mmap_lock);
            heap_mem = mm->brk - mm->start_brk;
            if (heap_mem > max_heap_mem) {
                max_heap_mem = heap_mem;
                pid = task->pid;
                tgid = task->tgid;
                strncpy(comm, task->comm, COMM_LEN);
            }
            up_read(&mm->mmap_lock);
        }
    }
    rcu_read_unlock();

    pr_info("TGID: %d | PID: %d | Name: %s has the maximum heap memory: %llu bytes\n", tgid, pid, comm, max_heap_mem);

    return 0; /* success */
}

/* This function is called when the module is removed. */
static void __exit lkm2_exit(void)
{
    pr_info("Exited from module %s\n", MODULE_NAME);
}

module_init(lkm2_init);
module_exit(lkm2_exit);

