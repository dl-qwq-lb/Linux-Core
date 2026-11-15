void _start(void)
{
    const char msg1[] = "syscall getuid started  \n\t";
    const char msg2[] = "UID returned as: ";
    const char msg3[] = "syscall exit started...  \n\t";
    const char msg4[] = "call exit(0)  \n\t";
    const char newline[] = "\n";
    
    long uid;
    
    // print msg1
    __asm__ volatile (
        "movl $4, %%eax\n\t"      
        "movl $1, %%ebx\n\t"      
        "movl %0, %%ecx\n\t"      
        "movl $26, %%edx\n\t"    
        "int $0x80\n\t"
        :
        : "r" (msg1)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // syscall getuid
    __asm__ volatile (
        "movl $24, %%eax\n\t"     
        "int $0x80\n\t"
        "movl %%eax, %0\n\t"      // save result
        : "=m" (uid)
        :
        : "%eax", "memory"
    );
    
    // get returned UID and convert them to str
    __asm__ volatile (
        // print msg2
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $15, %%edx\n\t"     
        "int $0x80\n\t"
        
        // get UID ASCIIed
        "movl %1, %%eax\n\t"      
        "addl $48, %%eax\n\t"     
        "pushl %%eax\n\t"         
        
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %%esp, %%ecx\n\t"   
        "movl $1, %%edx\n\t"     
        "int $0x80\n\t"
        
        "popl %%eax\n\t"          // clean stack
        :
        : "r" (msg2), "m" (uid)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // \n
    __asm__ volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $1, %%edx\n\t"
        "int $0x80\n\t"
        :
        : "r" (newline)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // print msg3
    __asm__ volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $26, %%edx\n\t"     
        "int $0x80\n\t"
        :
        : "r" (msg3)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // print msg4
    __asm__ volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl %0, %%ecx\n\t"
        "movl $20, %%edx\n\t"     
        "int $0x80\n\t"
        :
        : "r" (msg4)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // syscall exit
    __asm__ volatile (
        "movl $1, %%eax\n\t"      
        "movl $0, %%ebx\n\t"      
        "int $0x80\n\t"
        :
        :
        : "%eax", "%ebx", "memory"
    );
    
}