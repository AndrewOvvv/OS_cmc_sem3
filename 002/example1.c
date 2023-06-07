const char hello[] = "hello\n";

__attribute__((naked))
int _start()
{
    asm("mov $4, %eax\n"
        "mov $1, %ebx\n"
        "mov $hello, %ecx\n" 
        "mov $6, %edx\n"
        "int $0x80\n"
        "mov $1, %eax\n"
        "mov $55, %ebx\n"
        "int $0x80\n");
}
