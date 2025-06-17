#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ALLOC 1024

void hidden_logic(int argc, char *argv[]) {
    printf(">> Processing %d input arguments...\n", argc - 1);

    // Trigger 1: NULL dereference if specific character input
    if (argc > 1 && argv[1][0] == '@') {
        int *p = NULL;
        *p = 123;
        printf("Trigger 1 executed\n");
    }

    // Trigger 2: Out-of-bounds read
    if (argc > 3) {
        printf("Reading character: %c\n", argv[3][1]);  // May cause OOB read
    }

    // Trigger 3: Integer overflow leading to incorrect malloc
    if (argc > 2) {
        size_t n = strtoul(argv[2], NULL, 10);
        size_t size = n * sizeof(int);
        int *data = malloc(size);
        if (!data) {
            perror("malloc");
            return;
        }

        for (size_t i = 0; i <= n; i++) {  // CWE-787: OOB Write
            data[i] = (int)i;
        }

        free(data);

        // Trigger 4: Use-after-free
        printf("Post-free access: %d\n", data[0]);
    }
}

int main(int argc, char *argv[]) {
    printf("Welcome to the Black-box CWE Lab\n");
    printf("Provide inputs and observe behavior...\n");
    hidden_logic(argc, argv);
    printf("Execution complete\n");
    return 0;
}

