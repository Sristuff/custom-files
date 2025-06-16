
#include <stdio.h>
#include <string.h>

struct A {
    int id;
    float value;
    char label[8];
    double timestamp;
};

struct B {
    int id;
    float value;
    // Missing label field!
    double timestamp;
};

void bad_copy(struct A *a, struct B *b) {
    memcpy(b, a, sizeof(struct A)); // Unsafe: struct B is smaller than struct A
    printf("B: id = %d, value = %f, timestamp = %f\n", b->id, b->value, b->timestamp);
}

int main() {
    struct A a = {42, 3.14f, "DEMO", 168732.123};
    struct B b;
    bad_copy(&a, &b);  // May cause memory corruption
    return 0;
}

