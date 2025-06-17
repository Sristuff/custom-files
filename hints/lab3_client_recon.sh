#!/bin/bash
TARGET=$1
echo "[*] Running nmap scan..."
nmap -sV -p- $TARGET
