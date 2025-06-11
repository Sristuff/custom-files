/* This is meant for aarch64 plat only */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void actual() {
    printf("\nIntended function called. No control flow hijacking.\n\n");
}

void hacked() {
    printf("Current:PID:%d\n", getpid());
    printf("Current:PPID:%d\n", getppid());
    printf("Control flow hijacked! Spawning shell.\n");
    int ret = system("echo 'Inside shell:' && ps -p $$ -o pid, ppid && /bin/sh");
    if (ret == -1)
        printf("Shell: spawn failed\n");
    else 
        printf("Shell: exited: ret:%d\n", ret);
}

struct myframe {
    char buffer[32];
    void (*func_ptr)();
};

void vuln() {
    struct myframe f;
    f.func_ptr = actual;

    printf("Address of fncptr:%p\n", &f.func_ptr);
    printf("Address of buffer:%p\n", &f.buffer);
    printf("Address of actual():%p\n", &actual);
    printf("Address of hacked():%p\n", &hacked);

    printf("Enter input:");
    fgets(f.buffer, 128, stdin);
    //solution: fgets(f.buffer, sizeof(f.buffer), stdin);

    printf("You entered: %s\n", f.buffer);
    f.func_ptr();
}

int main() {
    vuln();
    return 0;
}
