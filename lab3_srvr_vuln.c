#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void greet(char *name) {
    char buf[100];
    strcpy(buf, name);
    printf("Hello, %s\n", buf);
}

int main(int argc, char *argv[]) {
    char name[512];
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    greet(name);
    return 0;
}
