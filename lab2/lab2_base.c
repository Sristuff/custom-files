#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>

#define BUF_SIZE 40

typedef void (*func_ptr_t)(int);
char input[256];

void hacked(int client_fd) {
    char msg[128];
    snprintf(msg, sizeof(msg), "[!] CONTROL FLOW HIJACKED\n[*] PID: %d, PPID: %d\n", getpid(), getppid());
    write(client_fd, msg, strlen(msg));

    char obfuscated[] = { 'W' ^ 0x23, 'a' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23, 'i' ^ 0x23, 'n' ^ 0x23, 'g' ^ 0x23,
                          ' ' ^ 0x23, 'f' ^ 0x23, 'o' ^ 0x23, 'r' ^ 0x23, ' ' ^ 0x23, 'o' ^ 0x23, 'u' ^ 0x23,
                          'r' ^ 0x23, ' ' ^ 0x23, 's' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, 'c' ^ 0x23, 'k' ^ 0x23,
                          ' ' ^ 0x23, 't' ^ 0x23, 'o' ^ 0x23, ' ' ^ 0x23, 'h' ^ 0x23, 'i' ^ 0x23, 't' ^ 0x23,
                          ' ' ^ 0x23, '1' ^ 0x23, '0' ^ 0x23, '0' ^ 0x23,
                          '\0' };

    for (int i = 0; obfuscated[i] != '\0'; i++) {
        obfuscated[i] ^= 0x23;
    }

    snprintf(msg, sizeof(msg), "[*] FLAG: %s\n", obfuscated);
    write(client_fd, msg, strlen(msg));
}

void safe(int client_fd) {
    const char *msg = "[*] You reached the safe function.\n";
    write(client_fd, msg, strlen(msg));
}

struct myframe {
    char buffer[BUF_SIZE];
    void (*func_ptr)(int);
};
void handle_client(int client_fd) {
    struct myframe f;
    f.func_ptr = safe;

    const char *prompt = "Mini-CTF Hint:fn:hacked.Enter your data: ";
    write(client_fd, prompt, strlen(prompt));

    ssize_t len = read(client_fd, input, sizeof(input));
    if (len < 0) {
        perror("read");
        return;
    }

    memcpy(&f.buffer, input, len);
    //f.buffer[len] = '\0';  // Null terminate to avoid overflow when printed
	
    f.func_ptr(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    signal(SIGPIPE, SIG_IGN);

    srand(time(NULL));
    int port = 1024 + rand() % (65535 - 1024);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Mini-CTF server listening..\n");

    while (1) {
	int client_fd = accept(server_fd, NULL, NULL);
	if (client_fd < 0) {
            perror("accept");
	    continue;
        }

        handle_client(client_fd);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
