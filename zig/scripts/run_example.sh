#!/usr/bin/env bash
set -euo pipefail
if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <run-step-name>" >&2
  echo "Example: $0 run-lang-hello" >&2
  exit 1
fi
zig build "$1"

