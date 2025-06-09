// Use: gcc -m32 -fno-stack-protector -z execstack -no-pie vuln.c -o vuln
#include <stdio.h>
#include <string.h>

void vuln() {
    char a[32];
    printf("Enter input: ");
    gets(a);
    printf("You entered: %s\n", a);
}

int main() {
    vuln();
    return 0;
}
