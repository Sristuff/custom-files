
# Canary + ASLR Bypass Lab (AArch64 Ubuntu 24.10)

This lab demonstrates how to bypass stack canaries and ASLR using an information leak vulnerability.

## Build
```
make
```

## Run
```
./vuln
```

## Defenses Used
- Stack canaries
- ASLR
- PIE disabled to simplify base offset guessing
