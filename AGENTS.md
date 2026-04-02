# AGENTS.md

## Cursor Cloud specific instructions

This is a multi-language programming tutorial monorepo. Each top-level directory is an independent sub-project with its own build system.

### Key services and how to run them

| Language | Directory | Test command | Lint/Format |
|----------|-----------|-------------|-------------|
| Python (root) | `/workspace` | `python3 -m pytest tests/ -v` | `flake8 src/ --max-line-length=100`, `black --check src/ tests/` |
| JavaScript | `javascript/` | `npx vitest run` | `npx prettier --check .` |
| TypeScript | `typescript/` | `npx vitest run` | `npx prettier --check .` |
| Go | `go/` | `go test ./...` | `go vet ./...` |
| Rust | `rust/` | `cargo test` | `cargo clippy` (if installed) |
| C | `c/` | `make all` (individual binaries build; `make test-all` has pre-existing linker errors) | N/A |
| Python (sub) | `python/` | `python3 -m pytest python/tests/` (run from workspace root) | `black --check .`, `isort --check .` |

### Non-obvious caveats

- **PATH**: Python user-installed tools (`pytest`, `black`, `flake8`, etc.) are in `~/.local/bin`. This is added to PATH in `~/.bashrc`. If commands are not found, run `export PATH="$HOME/.local/bin:$PATH"`.
- **Pre-existing test failures**: The root Python tutorial has 5 pre-existing test failures in `test_control_flow.py`, `test_data_structures.py`, and `test_stdlib_examples.py`. These are bugs in the tutorial code/tests, not environment issues.
- **Pre-existing lint issues**: `flake8` and `black --check` report style issues in the root Python code. These are pre-existing.
- **rust-tutorial/**: Has a pre-existing `Cargo.toml` parse error (duplicate `[workspace]` key). Use `rust/` (which works) instead.
- **python/ subdirectory**: The `test_async_sum` test requires `pytest-asyncio` which is not in its `requirements.txt`. The other 2 tests pass.
- **TypeScript build**: `tsc` fails due to missing `@types/node`, but `vitest run` works fine for tests.
- **No Docker/databases needed**: This is a pure tutorial repo with no services to start.
