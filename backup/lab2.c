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

void vuln() {
    char buffer[32];
    void (*func_ptr)() = actual;

    printf("Enter input:");
    fgets(buffer, 128, stdin);

    printf("You entered: %s\n", buffer);
    func_ptr();
}

int main() {
    vuln();
    return 0;
}
