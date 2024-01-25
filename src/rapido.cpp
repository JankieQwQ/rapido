#include <iostream>

void rprint(const char* str,char endl='\n',int safe=0){
    #ifdef __GNUC__
        // 在 GUNC 环境下，关闭流同步，不使用endl换行的std::cout是最快的。
        if(!safe){
            std::ios::sync_with_stdio(false);
        }
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

void rnextline(){
    rprint('\n');
}

int rinput() {
    int x = 0, w = 1;
    char ch = getchar_unlocked();
    
    __asm__ (
        "push rbp\n"
        "mov rbp, rsp\n"
        "sub rsp, 16\n"
        "mov DWORD PTR [rbp-4], 0\n"
        "mov DWORD PTR [rbp-8], 1\n"
        "call getchar_unlocked\n"
        "mov BYTE PTR [rbp-9], al\n"
        "jmp .L2\n"
    ".L4:\n"
        "cmp BYTE PTR [rbp-9], 45\n"
        "jne .L3\n"
        "mov DWORD PTR [rbp-8], -1\n"
    ".L3:\n"
        "call getchar_unlocked\n"
        "mov BYTE PTR [rbp-9], al\n"
    ".L2:\n"
        "cmp BYTE PTR [rbp-9], 47\n"
        "jle .L4\n"
        "cmp BYTE PTR [rbp-9], 57\n"
        "jg .L4\n"
        "jmp .L5\n"
    ".L7:\n"
        "mov edx, DWORD PTR [rbp-4]\n"
        "mov eax, edx\n"
        "sal eax, 2\n"
        "add eax, edx\n"
        "add eax, eax\n"
        "mov edx, eax\n"
        "movsx eax, BYTE PTR [rbp-9]\n"
        "sub eax, 48\n"
        "add eax, edx\n"
        "mov DWORD PTR [rbp-4], eax\n"
        "call getchar_unlocked\n"
        "mov BYTE PTR [rbp-9], al\n"
    ".L5:\n"
        "cmp BYTE PTR [rbp-9], 47\n"
        "jle .L6\n"
        "cmp BYTE PTR [rbp-9], 57\n"
        "jle .L7\n"
    ".L6:\n"
        "mov eax, DWORD PTR [rbp-4]\n"
        "imul eax, DWORD PTR [rbp-8]\n"
        "leave\n"
        "ret\n"
    );

    return x * w;
}