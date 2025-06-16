
all: vuln

vuln: vuln.c
	gcc -o vuln vuln.c -fstack-protector-all -no-pie -z execstack -g

clean:
	rm -f vuln
