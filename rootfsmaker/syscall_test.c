#define SYS_WRITE 4
#define SYS_FORK 2
#define SYS_WAITPID 7
#define SYS_EXIT 1
#define SYS_GETPID 20
#define SYS_GETPPID 64
#define SYS_PRINTPROCESS 325

void _start(void)
{
    const char msg_start[] = "===== System Call Test Program =====\n\n";
    const char msg_test1[] = "[Test 1] Direct system call\n";
    const char msg_test2[] = "[Test 2] Process chain test\n";
    const char msg_fork_child[] = "  Child process calling...\n";
    const char msg_fork_parent[] = "  Parent process waiting...\n";
    const char msg_end[] = "===== Test Complete =====\n";
    const char newline[] = "\n";
    
    // test1: call it directly
    // first, print msgs
    asm volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $37, %%edx\n\t"     
        "int $0x80\n\t"
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %1, %%ecx\n\t"
        "movl $26, %%edx\n\t"    
        "int $0x80\n\t"
        :
        : "r" (msg_start), "r" (msg_test1)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // syscall printprocess
    asm volatile (
        "movl $325, %%eax\n\t"
        "int $0x80\n\t"
        :
        :
        : "%eax", "memory"
    );
    
    // \n
    asm volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $1, %%edx\n\t"
        "int $0x80\n\t"
        :
        : "r" (newline)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // test2: process chain
    // always print info first
    asm volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $27, %%edx\n\t"     
        "int $0x80\n\t"
        :
        : "r" (msg_test2)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // fork
    long child_pid;
    asm volatile (
        "movl $2, %%eax\n\t"
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"
        : "=m" (child_pid)
        :
        : "%eax", "memory"
    );
    
    if (child_pid == 0) {
        // child process
        asm volatile (
            "movl $4, %%eax\n\t"
            "movl $1, %%ebx\n\t"
            "movl %0, %%ecx\n\t"
            "movl $28, %%edx\n\t"
            "int $0x80\n\t"
            :
            : "r" (msg_fork_child)
            : "%eax", "%ebx", "%ecx", "%edx", "memory"
        );
        
        asm volatile (
            "movl $325, %%eax\n\t"
            "int $0x80\n\t"
            :
            :
            : "%eax", "memory"
        );
        
        // exit child
        asm volatile (
            "movl $1, %%eax\n\t"
            "movl $0, %%ebx\n\t"
            "int $0x80\n\t"
            :
            :
            : "%eax", "%ebx", "memory"
        );
    } else {
        // parent process
        asm volatile (
            "movl $4, %%eax\n\t"
            "movl $1, %%ebx\n\t"
            "movl %0, %%ecx\n\t"
            "movl $30, %%edx\n\t"
            "int $0x80\n\t"
            :
            : "r" (msg_fork_parent)
            : "%eax", "%ebx", "%ecx", "%edx", "memory"
        );
        
        // wait child
        asm volatile (
            "movl $7, %%eax\n\t"
            "movl %0, %%ebx\n\t"
            "movl $0, %%ecx\n\t"
            "movl $0, %%edx\n\t"
            "int $0x80\n\t"
            :
            : "r" (child_pid)
            : "%eax", "%ebx", "%ecx", "%edx", "memory"
        );
        
        // parent process syscall printprocess
        asm volatile (
            "movl $325, %%eax\n\t"
            "int $0x80\n\t"
            :
            :
            : "%eax", "memory"
        );
    }
    
    // print finish information
    asm volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $24, %%edx\n\t"
        "int $0x80\n\t"
        :
        : "r" (msg_end)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // exit
    asm volatile (
        "movl $1, %%eax\n\t"
        "movl $0, %%ebx\n\t"
        "int $0x80\n\t"
        :
        :
        : "%eax", "%ebx", "memory"
    );
}