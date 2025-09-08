#!/usr/bin/env bash
set -euo pipefail
if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <run-step-name>" >&2
  echo "Example: $0 run-lang-hello" >&2
  exit 1
fi
ZIG_BIN="$(dirname "$0")/../.tools/zig/zig"
if [[ -x "$ZIG_BIN" ]]; then
  "$ZIG_BIN" build "$1"
else
  zig build "$1"
fi

