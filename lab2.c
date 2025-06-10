#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void actual() {
    printf("\nIntended function called. No control flow hijacking.\n\n");
}

void hacked() {
    printf("Control flow hijacked! You got shell access.\n");
    system("/bin/sh");
}

void vuln() {
    char buffer[32];
    void (*func_ptr)() = actual;

    printf("Enter input:");
    fgets(buffer, 128, stdin);

    printf("You entered: %s", buffer);
    func_ptr();
}

int main() {
    vuln();
    return 0;
}
