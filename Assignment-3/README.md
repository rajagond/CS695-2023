# -------------------- Assignment #3 - Due 15/02/2023 ------------------------------
## --------------------------------Part 1--------------------------------------------
### 1. ioctl system call
-   a. Describe the arguments (and purpose of each) of an ioctl system call?
    Ans. `int ioctl (int __fd, unsigned long int __request, ...)`
    - `__fd`: file descriptor of the open file for which the operation is performed
    - `__request`: request code for the operation. It is a device-dependent request code
    - `...`: optional arguments for the operation to pass additional information to the system call.
             It is an untyped pointer to memory.  It's traditionally char *argp (from the days before void * was valid C)
    - ioctl system call perform the I/O control operation specified by REQUEST on FD. 
        One argument may follow, its presence and type depend on REQUEST. Also, Return value of ioctl
        system call depends on REQUEST.  Usually -1 indicates error

-   b. Draw a diagram listing the steps in the kernel and in the user space to register and use an ioctl call.
    Ans. 
    ```basic
          +-----------------------------------+
         |                                   |
         |        USER APPLICATION           |
         |                                   |
         +-----------------------------------+                                            USER SPACE
      (ioctl()) ^^         || (ioctl() request)
      (reply)   ||         vv
        -------------------------------------------------------------------------------------------------
            +------------------+   +----------------+
            | sys call handler |<-->| ioctl handler  |                       KERNEL SPACE
            +------------------+   +----------------+
                                     ||      ^^
                                     vv      || (ioctl() reply)
                            +------------------------+
                            | Device Driver operation |
                            +------------------------+
       ══════════════════════════════════════════
                                    ║║                 
                            ╔════════════════╗   
                            ║     device     ║
                            ╚════════════════╝
    ```
    - The ioctl() system call can be invoked from a user space to manipulate the underlying device parameters.
        1. User space application calls ioctl system call with the file descriptor of the open file and the request code.
        2. The ioctl system call checks the file descriptor and request code and then calls the ioctl function in the kernel.
        3. The ioctl function in the kernel checks the request code and then calls the appropriate function to perform the operation.
        4. The function in the kernel performs the operation and returns the result to the ioctl function in the kernel.
        5. The ioctl function in the kernel returns the result to the ioctl system call.
        6. The ioctl system call returns the result to the user space application.

### 2. KVM API

-    a. What KVM API calls does the hypervisor use to set up and run the guest OS?
    Ans. A VM needs user memory region and virtual CPU(s), so all we need is to create VM, set up user memory region, 
        create vCPU(s) and its working space then execute it!
        KVM APIs used to set up and run the guest OS
        1. `KVM_CREATE_VM` - creates a new VM and /* returns a VM fd */.
        2  `KVM_SET_TSS_ADDR` - sets the address of the TSS for the VM.  Parameters: `unsigned long tss_address (in)`
                Returns: 0 on success, -1 on error
           + This ioctl is required on Intel-based hosts. This is needed on Intel hardware because of a quirk in 
           the virtualization implementation 
        2. `KVM_SET_USER_MEMORY_REGION `- sets up a memory region for the VM guest.  Set up memory for VM guest
           It takes a struct kvm_userspace_memory_region as an argument.
        3. `KVM_CREATE_VCPU` - creates a new vCPU for the VM.  It returns a vCPU fd.
        4. `KVM_GET_VCPU_MMAP_SIZE` - The KVM_RUN ioctl communicates with userspace via a shared memory region. 
            This ioctl returns the size of that region.Get size for mmap(vcpu_fd).(setup up memory for vCPU)
           -  Next we do the mmap call(Not a KVM API) to map the VCPU file descriptor(vcpufd) into the address space of the calling
            process with PROT_READ and PROT_WRITE protection flags, allowing both reading and writing to the mapped memory region. 
            The MAP_SHARED flag indicates that changes made to the mapped memory region will be visible to other processes 
            that have also mapped the same file.
           - The return value of the mmap call is a pointer to the start of the mapped memory region, which is stored in the kvm_run structure pointer.
        5. KVM_RUN - executes the vCPU.  It takes a struct kvm_run as an argument. Returns: 0 on success, -1 on error


-   b. List all the KVM API calls that the kvm-hello-world hypervisor uses and their purpose.
    Ans. 
    - `KVM_CREATE_VM, KVM_SET_TSS_ADDR, KVM_SET_USER_MEMORY_REGION, KVM_CREATE_VCPU, KVM_GET_VCPU_MMAP_SIZE, KVM_RUN`
        -Refer to the answer of question 2a for the purpose of each KVM API call.
    - `KVM_GET_API_VERSION` - returns the constant KVM_API_VERSION.  Parameters: None. Returns: KVM API version
    - `KVM_GET_REGS` - Reads the general purpose registers from the vcpu. Parameters: struct kvm_regs *regs (out)
        - Returns 0 success, -1 on error
        - X86_64: 
            ```cpp
                struct kvm_regs {
                    /* out (KVM_GET_REGS) / in (KVM_SET_REGS) */
                    __u64 rax, rbx, rcx, rdx;
                    __u64 rsi, rdi, rsp, rbp;
                    __u64 r8,  r9,  r10, r11;
                    __u64 r12, r13, r14, r15;
                    __u64 rip, rflags;
                };
            ```
        - `KVM_GET_SREGS` - Reads the special registers from the vcpu. Parameters: `struct kvm_sregs *sregs (out)`
            - Returns 0 success, -1 on error
            - X86_64:
                ```cpp
                struct kvm_sregs {
                    struct kvm_segment cs, ds, es, fs, gs, ss;
                    struct kvm_segment tr, ldt;
                    struct kvm_dtable gdt, idt;
                    __u64 cr0, cr2, cr3, cr4, cr8;
                    __u64 efer;
                    __u64 apic_base;
                    __u64 interrupt_bitmap[(KVM_NR_INTERRUPTS + 63) / 64];
                };
                ```
        - `KVM_SET_REGS` - Writes the general purpose registers to the vcpu. Parameters: `struct kvm_regs *regs (in)`
            - Returns 0 success, -1 on error
        - `KVM_SET_SREGS` - Writes the special registers to the vcpu. Parameters: `struct kvm_sregs *sregs (in)`
            - Returns 0 success, -1 on error
        

### 3. GVA -> HPA mapping in KVM(MMU)
```basic
GVA - Guest Virtual Address
HPA - Host Physical Address
```
-   a. Study these functions associated with these modes and explain how GVA is mapped to HPA in each of these modes.
    Ans.
    - Real Mode:
        - GVA is directly mapped to HPA. No translation is required. 
        - The GVA is the same as the HPA.
    - Protected Mode:
        - NO Paging is used. GVA is directly mapped to HPA.
    - Paged 32-bit Mode:
        - A single 4MB page to cover the memory region.
        - cr3 points to the pd_addr
    - Long Mode:
        - Guest Page table has 3 levels. pml4, pdpt, pdt.
        - cr3 points to the pml4 table.
        - `CR4_PAE` is set. 2MB pages are used. 21 bits are used for the page offset.
        - pml4 table has 512 entries. 9 bits are used for the index.
        - pdpt table has 512 entries. 9 bits are used for the index.
        - pdt table has 512 entries. 9 bits are used for the index.

        The guest's page table simply computes the physical address as exactly equal to the virtual address, 
        so that the guest's entire physical address space can be addressed directly without any concept of virtual addressing.

-   b. What is the size of the memory allocated to the guest VM in long mode?
    Ans. mem_size - passed as an argument to vm_init() function.  It is the size of the memory allocated to the guest VM.
         `mem_size = 0x200000 (2MB)`

-   c. Which code line inside the hypervisor sets up the guest’s page table? What is the KVM API call used to make the 
        KVM aware of the guest’s page table?
    Ans.
    The code line inside the hypervisor "map the memory" is in the function vm_init() in the file kvm-hello-world.c.
    -   ```cpp
        vm->mem = mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
        madvise(vm->mem, mem_size, MADV_MERGEABLE);
        ...
        ret = ioctl(vm->vm_fd, KVM_SET_USER_MEMORY_REGION, &memreg);
        ```
    - In long mode, the guest's page table happening with the code given below.
        ```cpp
        uint64_t pml4_addr = 0x2000;
        uint64_t *pml4 = (void *)(vm->mem + pml4_addr);
        uint64_t pdpt_addr = 0x3000;
        uint64_t *pdpt = (void *)(vm->mem + pdpt_addr);
        uint64_t pd_addr = 0x4000;
        uint64_t *pd = (void *)(vm->mem + pd_addr);
        pml4[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pdpt_addr;
        pdpt[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pd_addr;
        pd[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS;
        sregs->cr3 = pml4_addr;
        ```
    - The KVM API call used to make the KVM aware of the guest’s page table is `KVM_SET_USER_MEMORY_REGION`.
        -  `ioctl(vcpu->fd, KVM_SET_SREGS, &sregs)` 
            -  This call sets the CPU state for the virtual CPU vcpu with the structure regs.
               The structure regs contains the values for the various registers of the CPU, including the page table register,
               which holds the physical address of the guest's page table.
    
-   d. What is the role of CR4_PAE flag while setting the guest’s page table in the long mode?
    Ans. Reference: https://www.cse.iitb.ac.in/~mythili/virtcc/pa/pa1.html
        PAE - Physical Address Extension
    - Role of CR4_PAE: Enable / Disable the use of Physical Address Extension (PAE).
    - In the long mode, the physical address space of the guest can be up to 64-bits wide. 
    - When physical address extension (PAE) is enabled via the CR4_PAE flag, the page size is treated as 2MB, and the last 21 bits are 
          used to index into the pgdir. This allows for larger physical address spaces, and reduces the number of page table levels required
          to translate a virtual address to a physical address.

### 4. VM EXECUTION

-   a. At what (guest virtual) address does the guest start execution when it runs? Where is this address configured?
    Ans.
    -  `cs.base = 0;` is the address where the guest starts execution.
    -  This line of code is used to configure the starting address of the guest virtual machine's execution
-   b. At which line of code does the guest VM start execution, and what is the KVM API used to start the guest execution?
    Ans.
    - `KVM_RUN` is the KVM API used to start the guest execution.
    - `regs.rip = 0;` is the line of code where the guest VM starts execution.

### 5. The guest uses the outb function to write an 8-bit value into a serial port.

-   a. How is a value written to or read from a serial port in the guest.c program?
    Ans.
    - The guest uses the `outb` CPU instruction to write a byte of data to a certain I/O port number.
    - The guest uses the `inb` CPU instruction to read a byte of data from a certain I/O port number.
        Note that the actual I/O instruction of outb is written in assembly language and embedded into the C code, a technique called "Inline Assembly". 
    - This C function `outb` takes two arguments, a port number and a value.
-   b. How does the hypervisor access and print the string?
    Ans.

    - ```c
        char *p = (char *)vcpu->kvm_run;`
		fwrite(p + vcpu->kvm_run->io.data_offset, vcpu->kvm_run->io.size, 1, stdout);
        ```
    - The hypervisor accesses the string by reading the data from the guest memory using `p + vcpu->kvm_run->io.data_offset.`
    - The hypervisor prints the string by writing using fwrite() to the standard output stream.
-   c. Code in `guest.c` uses the value 42, what is this value used for?
    Ans.
    
    - ```c
      if (regs.rax != 42) {
      	printf("Wrong result: {E,R,}AX is %lld\n", regs.rax);
          return 0;
      }
      ```
    - guest code halt and put the value 42 in the RAX register. hypervisor received the `KVM_EXIT_HLT` and check the value of RAX.
    - If the value of `RAX` is 42 then the hypervisor knows HALT is called by the guest code.
    

## --------------------- Part 2 ---------------------

### How to run the code

1. Download the source code and extract it.

2. Open the terminal and go to the directory where the source code is extracted.

3. add the following files to the directory:

   1. `Makefile`
   2. `guest.ldt`
   3. `guest.s`
   4. `payload.ld`

4. Run the following commands:

   ```bash
   make clean
   make # result will be printed in the terminal
   ```