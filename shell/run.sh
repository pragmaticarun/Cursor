#!/usr/bin/env bash
set -euo pipefail

echo '=== Shell demo ==='
name=${1:-world}
echo "Hello, ${name}!"

# Arrays and loops
declare -a nums=(1 2 3 4 5)
sum=0
for n in "${nums[@]}"; do sum=$((sum+n)); done
echo "Sum=${sum}"

# Text processing
text='Hello hello world'
echo "$text" | tr 'A-Z' 'a-z' | grep -oE '[a-z]+' | sort | uniq -c

# List a few entries
ls -1 | head -3