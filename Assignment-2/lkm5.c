#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/io.h>

#define MODULE_NAME "lkm5"
#define COMM_LEN 16

MODULE_DESCRIPTION("LKM5");
MODULE_AUTHOR("Raja");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static unsigned int pid = 1;

module_param(pid, uint, 0660);

unsigned long long get_vma_size(struct task_struct *task)
{
    struct mm_struct *mm = task->mm;
    struct vm_area_struct *vma;
    unsigned long long vm_sz = 0;
    down_read(&mm->mmap_lock);
    for (vma = mm->mmap; vma != NULL; vma = vma->vm_next)
        vm_sz += (vma->vm_end - vma->vm_start);
    up_read(&mm->mmap_lock);
    return vm_sz;
}

unsigned long long get_phys_size(struct task_struct *task)
{
    struct mm_struct *mm = task->mm;
    struct vm_area_struct *vma;
    unsigned long long phy_sz = 0;
    unsigned long addr;
    down_read(&mm->mmap_lock);
    for (vma = mm->mmap; vma != NULL; vma = vma->vm_next)
    {
        // walk the page table to check if page is present
        for (addr = vma->vm_start; addr < vma->vm_end; addr += PAGE_SIZE)
        {
            pgd_t *pgd = pgd_offset(mm, addr);
            pte_t *pte = pte_offset_map(pmd_offset(pud_offset(p4d_offset(pgd, addr), addr), addr), addr);
            if (pte && pte_present(*pte))
                phy_sz += PAGE_SIZE;
        }
    }
    up_read(&mm->mmap_lock);
    return phy_sz;
}


/* This function is called when the module is loaded. */
static int __init lkm5_init(void)
{
    struct task_struct *task;
    char comm[COMM_LEN];
    unsigned long long vma_size = 0;
    unsigned long long phy_size = 0;
    
    pr_info("Module %s loaded...\n", MODULE_NAME);
    rcu_read_lock();
    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if (task) {
        strncpy(comm, task->comm, COMM_LEN);
        vma_size = get_vma_size(task);
        pr_info("TGID: %d | PID: %d | Name: %s | VMA Size: %llu bytes | VM Pages : %llu \n", task->tgid, task->pid, comm, vma_size, vma_size >> PAGE_SHIFT);
        phy_size = get_phys_size(task);
        pr_info("TGID: %d | PID: %d | Name: %s | Physical Size: %llu bytes | Physical Pages : %llu \n", task->tgid, task->pid, comm, phy_size, phy_size >> PAGE_SHIFT);

    } else {
        pr_warn("No task found for pid %d/n", pid);
    }
    // pr_info("%lu", task->mm->total_vm); // total_vm is the total number of pages in the process's virtual memory space
    rcu_read_unlock();
    return 0; /* success */
}

/* This function is called when the module is removed. */
static void __exit lkm5_exit(void)
{
    pr_info("Exited from module %s\n", MODULE_NAME);
}

module_init(lkm5_init);
module_exit(lkm5_exit);
