// 不引用任何头文件！纯 syscall 版 init

long syscall3(long n, long a, long b, long c) {
    long ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(n), "b"(a), "c"(b), "d"(c)
        : "memory"
    );
    return ret;
}

long syscall1(long n, long a) {
    long ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(n), "b"(a)
        : "memory"
    );
    return ret;
}

void _start() {
    const char msg[] = "Hello World from init !\n";

    syscall3(4, 1, (long)msg, sizeof(msg)-1); // SYS_write = 4

    while (1) {
        // SYS_nanosleep = 162
        long ts[2] = {1, 0};
        syscall3(162, (long)ts, 0, 0);
    }
}

