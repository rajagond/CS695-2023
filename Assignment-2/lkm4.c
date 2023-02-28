#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/io.h>
#define MODULE_NAME "lkm4"


static pid_t pid = 1;
static unsigned long virt_addr = 0xffff80000000000f;

module_param(pid, int, 0660);
module_param(virt_addr, ulong, 0660);

MODULE_DESCRIPTION("LKM4");
MODULE_AUTHOR("Raja");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

void print_physicalAddress(void) {
    struct task_struct *task; // Task struct for the given pid
    struct mm_struct *mm; // Memory map for the given task
    
    /* Unused | PGD | PUD | PMD | PTE | offset*/
    pgd_t *pgd; // Page Global Directory
    p4d_t *p4d; // Page 4th Directory
    pud_t *pud; // Page Upper Directory
    pmd_t *pmd; // Page Middle Directory
    pte_t *pte; // Page Table Entry

    phys_addr_t phys_addr;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if (!task) {
        pr_alert("Error: PID %d does not exist\n", pid);
        return;
    }
    mm = task->mm;
    if (!mm) {
        pr_alert("Error: PID %d does not have a memory map\n", pid);
        return;
    }
    if( virt_addr >= 0xffff800000000000){
        phys_addr = virt_to_phys((void *)virt_addr);
        if (!phys_addr) {
            pr_alert("Error: Virtual Address 0x%lx of PID: %d does not exist... \n", virt_addr, pid);
            return;
        }
        pr_info("PID: %d \t Virtual Address: 0x%lx \t Physical Address: 0x%llx \n", pid, virt_addr, phys_addr);
        return;
    }
    pgd = pgd_offset(mm, virt_addr);
    if(pgd_none(*pgd) || pgd_bad(*pgd)){
        pr_alert("Error - pgd: Virtual Address 0x%lx of PID %d is not present\n", virt_addr, pid);
        return;
    }
    p4d = p4d_offset(pgd, virt_addr);
    if(p4d_none(*p4d) || p4d_bad(*p4d)){
        pr_alert("Error - p4d: Virtual Address 0x%lx of PID %d is not present\n", virt_addr, pid);
        return;
    }
    pud = pud_offset(p4d, virt_addr);
    if(pud_none(*pud) || pud_bad(*pud)){
        pr_alert("Error - pud: Virtual Address 0x%lx of PID %d is not present\n", virt_addr, pid);
        return;
    }
    pmd = pmd_offset(pud, virt_addr);
    if(pmd_none(*pmd) || pmd_bad(*pmd)){
        pr_alert("Error - pmd: Virtual Address 0x%lx of PID %d is not present\n", virt_addr, pid);
        return;
    }

    pte = pte_offset_map(pmd, virt_addr);
    if(!pte_present(*pte)){
        pr_alert("Error - pte: Virtual Address 0x%lx of PID %d is not present\n", virt_addr, pid);
        return;
    }

    phys_addr = (pte_val(*pte) & PTE_PFN_MASK) | (virt_addr & ~PTE_PFN_MASK);
    pr_info("TGID: %d \t PID: %d \t Virtual Address: 0x%lx \t pfn: %lx \t Physical Address: 0x%llx\n", task->tgid, task->pid, virt_addr, pte_pfn(*pte), phys_addr);
    pte_unmap(pte);

    return;
}


static int __init lkm4_init(void)
{
    pr_info("Module %s loaded...\n", MODULE_NAME);

    print_physicalAddress();
   
    return 0;
}

static void __exit lkm4_exit(void)
{
    pr_info("Exited from module %s \n", MODULE_NAME);
}

module_init(lkm4_init);
module_exit(lkm4_exit);

