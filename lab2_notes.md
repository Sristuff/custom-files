## Files
- `lab2.c`: Vulnerable C source
- `Makefile`: Builds 3 variants:
  - `vuln`: Vulnerable build
  - `vuln_prot`: With stack protector
  - `vuln_asan`: With AddressSanitizer

## Setup
```bash
make
```

## Run
```bash
./vuln          # No protections
  - hint: Read and modify lab2_exploit.py
  - hint: python3 lab2_exploit.py to create input.text
  - hint: Verify by using hexdump -C ./input.txt
  - hint: ./vuln < input.txt
./vuln_prot # Stack protector enabled
./vuln_asan # AddressSanitizer
```

## Cleanup
```bash
make clean
```
