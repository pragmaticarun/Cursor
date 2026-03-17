# Easy: Rust Installation and Environment Setup

## What this feature is

Before learning language features, you need a stable toolchain and standard developer components. Rust uses `rustup` to install/manage compiler channels and related tools.

## Prerequisites

- Internet access
- Terminal/PowerShell access

## Installation steps

### Linux / macOS / WSL

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source "$HOME/.cargo/env"
rustc --version
cargo --version
```

### Windows (PowerShell)

```powershell
winget install Rustlang.Rustup
rustc --version
cargo --version
```

### Recommended components

```bash
rustup update
rustup component add rustfmt clippy
```

## Self-contained example

```rust
fn main() {
    println!("Rust setup successful!");
}
```

Run:

```bash
cargo new hello-rust
cd hello-rust
cargo run
```

## Best practices

- Keep stable toolchain updated.
- Enable `rustfmt` and `clippy` in your editor/CI.
- Pin toolchain in team repos when reproducibility matters.

## Pitfalls

- Mixing toolchains unintentionally across projects.
- Skipping `rustup update` for long periods.
