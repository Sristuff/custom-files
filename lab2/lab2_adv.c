#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>

struct frame {
    char buffer[32];
    void (*func_ptr)();
};

void safe() {
    printf("Connected, but no control flow change. Please try again.\n");
}

void hacked() {
    printf("[!] CONTROL FLOW HIJACKED\n");
    printf("[*] PID: %d, PPID: %d\n", getpid(), getppid());

    char obfuscated[] = { 'W' ^ 0x23, 'a' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23, 'i' ^ 0x23, 'n' ^ 0x23, 'g' ^ 0x23,
                          ' ' ^ 0x23, 'f' ^ 0x23, 'o' ^ 0x23, 'r' ^ 0x23, ' ' ^ 0x23, 'o' ^ 0x23, 'u' ^ 0x23,
                          'r' ^ 0x23, ' ' ^ 0x23, 's' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, 'c' ^ 0x23, 'k' ^ 0x23,
                          ' ' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, ' ' ^ 0x23, 'h' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23,
                          ' ' ^ 0x23, '2' ^ 0x23, '0' ^ 0x23, '0' ^ 0x23,
                          '\0' };

    for (int i = 0; obfuscated[i] != '\0'; i++) {
        obfuscated[i] ^= 0x23;
    }

    printf("[*] FLAG: %s\n", obfuscated);
}

void handle_client(int client_fd) {
    struct frame f;
    f.func_ptr = safe;

    write(client_fd, "Hint:string:CONTROL FLOW HIJACKED\n Enter data:", 47);
    read(client_fd, f.buffer, 128);  // CWE-787

    dup2(client_fd, STDOUT_FILENO);
    dup2(client_fd, STDERR_FILENO);
    close(client_fd);

    f.func_ptr();
}

int main() {
    srand(time(NULL));
    int port = (rand() % (65535 - 1024)) + 1024;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Mini-CTF server listening...\n");
    fflush(stdout);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd >= 0) {
            handle_client(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
