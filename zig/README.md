## Zig Tutorial Project

This repository contains a hands-on tutorial for Zig, with runnable examples covering core language features and commonly used standard library facilities.

### Prerequisites

- Zig 0.11 or newer installed and on your PATH

### Quick Start

- Build all executables: `zig build`
- Run all examples: `zig build run-all`
- Run tests: `zig build test`
- Format code: `zig build fmt` or `zig fmt .`

### Structure

- `src/examples/language/`: Core language feature examples
- `src/examples/stdlib/`: Standard library facility examples

Each example is a standalone `main` program and also contains `test` blocks you can run with `zig build test`.

### Running individual examples

Use the generated build steps:

- `zig build run-lang-hello`
- `zig build run-std-io-print`

List more run steps with `zig build -l | grep run-`.

