#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>

#define BUF_SIZE 40

struct frame {
    char buffer[32];
    void (*func_ptr)();
};

void safe(int client_fd) {
    const char *msg = "Connected. Safe function. Please try again\n";
    write (client_fd, msg, strlen(msg));
}

void hacked(int client_fd) {

    char obfuscated[] = { 'W' ^ 0x23, 'a' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23, 'i' ^ 0x23, 'n' ^ 0x23, 'g' ^ 0x23,
                          ' ' ^ 0x23, 'f' ^ 0x23, 'o' ^ 0x23, 'r' ^ 0x23, ' ' ^ 0x23, 'o' ^ 0x23, 'u' ^ 0x23,
                          'r' ^ 0x23, ' ' ^ 0x23, 's' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, 'c' ^ 0x23, 'k' ^ 0x23,
                          ' ' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, ' ' ^ 0x23, 'h' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23,
                          ' ' ^ 0x23, '1' ^ 0x23, '0' ^ 0x23, '0' ^ 0x23,
                          '\0' };

    for (int i = 0; obfuscated[i] != '\0'; i++) {
        obfuscated[i] ^= 0x23;
    }

    write(client_fd, obfuscated, strlen(obfuscated));

    //printf("[*] FLAG: %s\n", obfuscated);
}

void handle_client(int client_fd) {
    char buffer[BUF_SIZE];

    struct frame f;
    f.func_ptr = safe;

    write(client_fd, "Hint:fn:hacked\n Enter your data: ", 34);
    read(client_fd, f.buffer, 128);  // CWE-787

    f.func_ptr(client_fd);

    dup2(client_fd, STDOUT_FILENO);
    dup2(client_fd, STDERR_FILENO);
    close(client_fd);

   // f.func_ptr();
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
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
