
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void hacked() {
    printf("[!] This function should never be reachable in secure code.\n");
    printf("PID: %d, PPID: %d\n", getpid(), getppid());
}

void secure_input(char *buf, size_t size) {
    if (fgets(buf, size, stdin) == NULL) {
        perror("fgets failed");
        exit(1);
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
}

void safe_printf(const char *msg) {
    printf("%s\n", msg);
}

void secure_function() {
    char buf[64];
    printf("Enter your input:\n");
    secure_input(buf, sizeof(buf));
    safe_printf(buf);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    secure_function();
    return 0;
}
