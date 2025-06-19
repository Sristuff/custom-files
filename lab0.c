#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

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

const char *top6[] = {"123456", "123456789", "12345678","password","qwerty"};
const char *usual[] = {"cisco123", "test123", "lab123", "cisco","C!sco123"};
const char *cheeky[] = {"crash", "die", "DEADBEEF", "fail", "segfault", "core"};
const char *good[] = {"CWE", "cwe", "CVE", "cve"};

void hidden_logic(int argc, char *argv[]) {
    char at_sym[2], magic_val[5];
    build_str(at_sym, "\x1A", 1);        // '@'
    build_str(magic_val, "\x39\x38\x38\x38", 4); // "1000"

    if (argc < 2) {
        printf("Not enough arguments.\n");
        return;
    }

    if (argc > 2) {
        printf("Too many arguments.\n");
        return;
    }

    for (int i = 0; i < 6; i++) {
        if (strncmp(argv[1], cheeky[i], strlen(cheeky[i])) == 0) {
            printf("Trying clever ones? Good try, but no triggers here!\n");
            cwe_counter += .1;
            return;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (strncmp(argv[1], top6[i], strlen(top6[i])) == 0) {
            printf("Trying Top 6 passwords? Good try, but no triggers here!\n");
            cwe_counter += .1;
            return;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (strncmp(argv[1], usual[i], strlen(usual[i])) == 0) {
            printf("Trying company defaults? Good try, but no triggers here!\n");
            cwe_counter += .2;
            return;
        }
    }
    
    for (int i = 0; i < 4; i++) {
        if (strncmp(argv[1], good[i], strlen(good[i])) == 0) {
            printf("Close....no triggers here, but try again!\n");
            cwe_counter += .3;
            return;
        }
    }
    
    if (argc == 2) {
        char *endptr = NULL;
        size_t n = strtoul(argv[1], &endptr, 0);
        if (errno == ERANGE) {
            printf("Interesting range input, but no triggers here!\n");
            cwe_counter += .2;
            return;
        } else if (*endptr != '\0') {
            printf("No triggers hit. Please try again. Current CWE score:%f\n", cwe_counter);
            return;
        }

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
        } else if (n == 787) {
            printf("Trigger:Out-of-bounds write triggered!\n");
            cwe_counter += 2;
            printf("Total CWE triggers hit:%f\n", cwe_counter);
            printf("Reading input:%s\n", argv[1]);
            printf("Writing one byte beyond input:\n");
            argv[1][3] = 'a';
            printf("Reading one byte beyond the input:[%c,%c,%c,%c]\n", argv[1][0], argv[1][1], argv[1][2], argv[1][3]);
        } else if (n == 190) {
            printf("Trigger:Integer overflow triggered!\n");
            cwe_counter += 2;
            size_t n = SIZE_MAX;
            printf("Assigning SIZE_MAX to n:%lu\n", n);
            n++;
            printf("Value of n after n++ is:%lu\n", n);
        } else if (n == SIZE_MAX) {
            size_t size = n * sizeof(int);
            if (n > 0 && size / n != sizeof(int)) {
                printf("Trigger:Integer overflow in malloc!\n");
                cwe_counter++;
                return;
            }
        } else if ((n == INT_MIN) || (n == CHAR_MIN) || (n == LONG_MIN) || (n == SHRT_MIN) || (n == LLONG_MIN)) {
            printf("Interesting input with MIN values, but no triggers here!\n");
            cwe_counter += .2;
            return;
        } else if ((n == INT_MAX) || (n == CHAR_MAX) || (n == LONG_MAX) || (n == SHRT_MAX) || (n == LLONG_MAX) || (n == USHRT_MAX) || (n == UCHAR_MAX)) {
            printf("Interesting input with MAX values, but no triggers here!\n");
            cwe_counter += .2;
            return;
        } else if ((n == 0) || (n == 1)) {
            printf("Interesting input, but no triggers here!\n");
            cwe_counter += .1;
            return;
        } else {
            printf("No triggers hit. Please try again. Current CWE score:%f\n", cwe_counter);
            return;
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    printf("______________________________________________________________\n");
    printf("Welcome to the Interactive Black-box Lab (type `exit` to quit)\n");
    printf("______________________________________________________________\n");

    char input[MAX_INPUTS][MAX_LEN];
    char *argv[MAX_INPUTS + 1];
    int argc;

    while (1) {
        printf("\n> Put on your thinking hat! Try different inputs (or type `exit` to finish):\n> ");
        char line[MAX_LEN * MAX_INPUTS] = {0};
        if (!fgets(line, sizeof(line), stdin)) break;

        if (strncmp(line, "help", 4) == 0) {
            printf("______________________________________________________________\n");
            printf("Try various alphanumeric inputs to trigger a CWE and get points");
            printf("______________________________________________________________\n");
            continue;
        }

        if (strncmp(line, "exit", 4) == 0) {
            printf("________________________________________\n");
            printf("Lab complete. Total CWE score: %f\n", cwe_counter);
            printf("________________________________________\n");
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
