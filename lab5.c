#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ALLOC 1024
#define MAX_INPUTS 10
#define MAX_LEN 256
#define XOR_KEY 0x5A

float cwe_counter = 0;

// Runtime string deobfuscation
void build_str(char *dest, const char *src, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dest[i] = src[i] ^ XOR_KEY;
    }
    dest[len] = '\0';
}

int is_usual_pass(const char *s) {
    return strstr(s, "cisco") || 
           strstr(s, "cisco123") || 
           strstr(s, "lab") || 
           strstr(s, "lab123");
           strstr(s, "C!sco123");
}

int is_suspicious(const char *s) {
    return strstr(s, "crash") ||
           strstr(s, "CWE") ||
           strstr(s, "die") ||
           strstr(s, "DEADBEEF") ||
           strstr(s, "fail");
}

void hidden_logic(int argc, char *argv[]) {
    char at_sym[2], magic_val[5];
    build_str(at_sym, "\x1A", 1);        // '@'
    build_str(magic_val, "\x39\x38\x38\x38", 4); // "1000"

    if (argc < 2) {
        printf("Not enough arguments.\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        if (is_suspicious(argv[i])) {
            printf("Trying '%s'? Sneaky move!\n", argv[i]);
        }

        if (is_usual_pass(argv[i])) {
            printf("Trying company defaults? Good try!\n");
            cwe_counter += .2;
        }
    }

    if (argc == 2) {
        int n = strtoul(argv[1], NULL, 0);
        if (n == 476) {
            printf("Trigger:NULL pointer dereference triggered!\n");
            cwe_counter += 3;
            printf("Going down! Total CWE triggers hit:%f\n", cwe_counter);
            int *p = NULL;
            *p = 123;
        } else if (n == 416) {
            printf("Trigger:Use after free triggered!\n");
            cwe_counter += 2;
            printf("Total CWE triggers hit:%f\n", cwe_counter);
            int *data = malloc(n);
            data[0] = n;
            printf("Data at data[0]:%p before-free is %d\n", &data[0], data[0]);
            free(data);
            printf("Data at data[0]:%p use-after-free is %d\n", &data[0], data[0]);
            data = NULL;
            printf("Address of data after data=NULL:%p\n", &data[0]);
        } else if (n == 125) {
            printf("Trigger:Out-of-bounds read triggered!\n");
            cwe_counter += 2;
            printf("Total CWE triggers hit:%f\n", cwe_counter);
            printf("Reading input:%s\n", argv[1]);
            printf("Reading one byte beyond the input:[%c,%c,%c,%c]\n", argv[1][0], argv[1][1], argv[1][2], argv[1][3]);
        }
        return;
    }

    if (argc == 3) {
        size_t n = strtoul(argv[2], NULL, 0);
        if (n == SIZE_MAX) {
            size_t size = n * sizeof(int);
            if (n > 0 && size / n != sizeof(int)) {
                printf("Trigger:Integer overflow in malloc!\n");
                cwe_counter++;
                return;
            }
        }
    }

    if (argc > 6 && strcmp(argv[6], "UNINIT") == 0) {
        int x;
        printf("🎲 Using uninitialized value: %d\n", x);
        cwe_counter++;
    }

    if (argc > 7 && strlen(argv[7]) > 10) {
        char buf[10];
        strcpy(buf, argv[7]);
        printf("🔥 Buffer overflow with strcpy!\n");
        cwe_counter++;
    }
 
    printf("No triggers hit. Please try again\n");
}

int main() {
    srand((unsigned)time(NULL));
    printf("Welcome to the Interactive CWE Black-box Lab (type `exit` to quit)\n");

    char input[MAX_INPUTS][MAX_LEN];
    char *argv[MAX_INPUTS + 1];
    int argc;

    while (1) {
        printf("\n> Enter space-separated arguments (or type `exit` to finish):\n> ");
        char line[MAX_LEN * MAX_INPUTS] = {0};
        if (!fgets(line, sizeof(line), stdin)) break;

        if (strncmp(line, "help", 4) == 0) {
            printf("First paramater value can crash the program.\n");
            printf("Second parameter value relates to memory allocation");
            continue;
        }

        if (strncmp(line, "exit", 4) == 0) {
            printf("Lab complete. Total CWE triggers hit: %f\n", cwe_counter);
            break;
        }

        argc = 1; // argv[0] is dummy
        argv[0] = "lab";
        char *token = strtok(line, " \t\n");
        while (token && argc < MAX_INPUTS + 1) {
            strncpy(input[argc - 1], token, MAX_LEN - 1);
            input[argc - 1][MAX_LEN - 1] = '\0';
            argv[argc] = input[argc - 1];
            argc++;
            token = strtok(NULL, " \t\n");
        }

        hidden_logic(argc, argv);
    }

    return 0;
}
