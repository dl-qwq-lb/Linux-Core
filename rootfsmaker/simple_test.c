void _start(void)
{
    // print msg1
    asm volatile (
        "movl $4, %%eax\n\t"     
        "movl $1, %%ebx\n\t"      
        "movl $message1, %%ecx\n\t"
        "movl $20, %%edx\n\t"    
        "int $0x80\n\t"
        :
        :
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    
    // syscall my new call: printprocess
    asm volatile (
        "movl $325, %%eax\n\t"    
        "int $0x80\n\t"
        :
        :
        : "%eax", "memory"
    );
    
    // print msg2
    asm volatile (
        "movl $4, %%eax\n\t"
        "movl $1, %%ebx\n\t"
        "movl $message2, %%ecx\n\t"
        "movl $18, %%edx\n\t"    
        "int $0x80\n\t"
        :
        :
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
    
    // data part
    asm volatile (
        "jmp .string_end\n\t"
        "message1: .ascii \"Testing syscall...\\n\"\n\t"
        "message2: .ascii \"Test complete.\\n\"\n\t"
        ".string_end:\n\t"
    );
}