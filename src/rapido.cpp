#include <iostream>

void rprint(const char* str,char endl='\n'){
    #ifdef __GNUC__
        // 在 GUNC 环境下，关闭流同步，不使用endl换行的std::cout是最快的。
        std::ios::sync_with_stdio(false);
        std::cout << str << endl;
    #else 
        __asm__(
        "push    rbp\n"
        "mov     rbp, rsp\n"
        "sub     rsp, 16\n"
        "mov     QWORD PTR [rbp-8], rdi\n"
        "mov     rdx, QWORD PTR stdout[rip]\n"
        "mov     rax, QWORD PTR [rbp-8]\n"
        "mov     rsi, rdx\n"
        "mov     rdi, rax\n"
        "call    fputs\n"
        "mov     edi, 10\n"
        "call    putchar\n"
        "nop\n"
        "leave\n"
        "ret\n"
        );
    #endif
}

int rinput() {
    int x = 0, w = 1;
    char ch = getchar_unlocked();
    
    while (ch < '0' || ch > '9') {
        if (ch == '-') w = -1;
        ch = getchar_unlocked();
    }
    
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = getchar_unlocked();
    }
    
    return x * w;
}