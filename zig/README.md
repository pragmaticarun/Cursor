## Zig Tutorial Project

This repository contains a hands-on tutorial for Zig, with runnable examples covering core language features and commonly used standard library facilities.

### Prerequisites

- Zig 0.14 (project bootstraps a local toolchain automatically)

### Quick Start

- Build all executables: `make build`
- Run all examples: `make run-all`
- Run tests: `make test`
- Format code: `make fmt`

### Structure

- `src/examples/language/`: Core language feature examples
- `src/examples/stdlib/`: Standard library facility examples

Each example is a standalone `main` program and also contains `test` blocks you can run with `zig build test`.

### Running individual examples

Use the generated build steps:

- `zig build run-lang-hello`
- `zig build run-std-io-print`

List more run steps with `zig build -l | grep run-`.

