#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
void disable_sync() {
    std::ios::sync_with_stdio(false);
}

void rprint(const char* str,char endl='\n',int safe=0) {
    #ifdef __GNUC__
        if(!safe){
            std::cout << str << endl;
        }else{
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
                "leave\n"
                "ret\n"
        );
        }
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
        "leave\n"
        "ret\n"
        );
    #endif
}

void rnextline() {
    rprint("\n");
}

size_t strlen(const char* str) {
    #ifdef __GNUC__
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
    #else
    __m128i zero = _mm_setzero_si128();
    __m128i ones = _mm_set1_epi8(1);
    int i;
    for (i = 0; str[i] != '\0'; i += 16) {
        __m128i chars = _mm_loadu_si128((__m128i*)(str + i));
        __m128i cmp = _mm_cmpeq_epi8(chars, zero);
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0) {
            int pos = __builtin_ctz(mask);
            return i + pos;
        }
    }
    while (str[i] != '\0') {
        i++;
    }
    return i;
    #endif
}

void* rfileinput(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return nullptr;
    }
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        close(fd);
        return nullptr;
    }
    off_t fileSize = fileStat.st_size;
    void* data = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return nullptr;
    }
    close(fd);
    return data;
}
bool rfileoutput(const char* str, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return false;
    }
    size_t len = strlen(str);
    size_t written = fwrite(str, 1, len, file);
    if (written != len) {
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}
int rinput(int safe=0) {
    int x = 0, w = 1;
    char ch = getchar_unlocked();
    #ifdef __GNUC__
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
    #else
        if(!safe){
        while (ch < '0' || ch > '9') {
            if (ch == '-') w = -1;
            ch = getchar_unlocked();
        }
    
        while (ch >= '0' && ch <= '9') {
            x = x * 10 + (ch - '0');
            ch = getchar_unlocked();
        }}else{
        while (ch < '0' || ch > '9') {
            if (ch == '-') w = -1;
            ch = getchar();
        }
    
        while (ch >= '0' && ch <= '9') {
            x = x * 10 + (ch - '0');
            ch = getchar();
        }
    #endif
    return x * w;
}

