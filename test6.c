
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct A {
    int id;
    float value;
    char label[4];
    char timestamp;
};

struct B {
    int id;
    float value;
    // Intentionally missing label[32]
    char timestamp;
};

void cause_crash(struct A *a) {
    // Allocate memory only for struct B (smaller)
    struct B *b = (struct B *)malloc(sizeof(struct B));
    if (!b) {
        perror("malloc failed");
        exit(1);
    }

    // Overwrite memory beyond allocated size (undefined behavior)
    memcpy(b, a, sizeof(struct A));  // Dangerous copy

    // Try accessing the "label" field in struct B's memory space
    // This is beyond valid allocated memory
    printf("Fake label read: %s\n", ((char *)b) + offsetof(struct A, label));

    // Optional: write to the out-of-bounds region (very likely to crash)
    strcpy(((char *)b) + offsetof(struct A, label), "TEST_WRITE");

    free(b);
}

int main() {
    struct A a = {1234, 9.81f, "BOOM", 9};
    cause_crash(&a);
    return 0;
}

