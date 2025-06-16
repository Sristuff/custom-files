
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void hacked() {
    printf("[*] HACKED! You've bypassed ASLR and stack canary.\n");
    printf("PID: %d, PPID: %d\n", getpid(), getppid());
    system("/bin/sh");
}

void vulnerable() {
    char buf[64];
    printf("Enter your input:\n");
    fgets(buf, 192, stdin); // Intentional overread
    printf(buf); // Format string vulnerability
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    vulnerable();
    return 0;
}
