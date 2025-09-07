# Shell Tutorial: Bash Essentials and Core Utilities

## Quick start

```bash
bash shell/run.sh
```

## Basics

- Variables, quoting, command substitution `$(...)`
- Conditionals: `if`, `[[ ]]`
- Loops: `for`, `while`, `until`
- Functions; `set -euo pipefail`
- Pipelines and redirection, here-docs

## Core utilities

- Text: `grep`, `sed`, `awk`, `cut`, `tr`, `sort`, `uniq`, `wc`
- Files: `find`, `xargs`, `rsync`, `tar`
- Networking: `curl`, `dig`, `nc`
- JSON/YAML: `jq`, `yq` (external tools)

## Best practices

- Quote variables, prefer arrays for word-safety
- Use `trap` for cleanup, and `mktemp` for temp files

See `shell/run.sh` for runnable examples.