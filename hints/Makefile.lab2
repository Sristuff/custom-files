CC=gcc
CFLAGS=-O0 -g -fno-stack-protector
PROTECTED_FLAGS=-O2 -fstack-protector-strong
ASAN_FLAGS=-O1 -fsanitize=address -fno-omit-frame-pointer

all: vuln protected asan_vuln

vuln: lab2.c
	$(CC) $(CFLAGS) lab2.c -o vuln

protected: lab2.c
	$(CC) $(PROTECTED_FLAGS) lab2.c -o vuln_prot

asan_vuln: lab2.c
	$(CC) $(ASAN_FLAGS) lab2.c -o vuln_asan

clean:
	rm -f vuln vuln_prot vuln_asan 
