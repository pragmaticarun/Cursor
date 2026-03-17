# Rust Features Tutorial (Split by Concept)

This is the split version of the Rust tutorial: one Markdown file per concept/feature, organized by difficulty.

- **Easy**: foundational syntax and core ownership model
- **Medium**: abstraction, error architecture, and tooling discipline
- **Hard**: advanced runtime patterns and language power features
- **Brutal**: low-level systems topics where correctness pressure is highest

All code examples are self-contained (each can live in its own `main.rs`), and representative examples are validated in:

```bash
cargo test --test rust_tutorial_examples
```

---

## Easy

1. [`00_rust_installation.md`](./easy/00_rust_installation.md)
2. [`01_variables_mutability_shadowing.md`](./easy/01_variables_mutability_shadowing.md)
3. [`02_types_control_flow.md`](./easy/02_types_control_flow.md)
4. [`03_ownership_borrowing_slices.md`](./easy/03_ownership_borrowing_slices.md)
5. [`04_structs_enums_pattern_matching.md`](./easy/04_structs_enums_pattern_matching.md)
6. [`05_modules_collections_option_result.md`](./easy/05_modules_collections_option_result.md)

## Medium

1. [`01_generics_traits_lifetimes.md`](./medium/01_generics_traits_lifetimes.md)
2. [`02_iterators_closures.md`](./medium/02_iterators_closures.md)
3. [`03_custom_errors_question_mark.md`](./medium/03_custom_errors_question_mark.md)
4. [`04_testing_tooling_cfg.md`](./medium/04_testing_tooling_cfg.md)

## Hard

1. [`01_smart_pointers.md`](./hard/01_smart_pointers.md)
2. [`02_concurrency_threads_channels_mutex.md`](./hard/02_concurrency_threads_channels_mutex.md)
3. [`03_async_await.md`](./hard/03_async_await.md)
4. [`04_macros.md`](./hard/04_macros.md)
5. [`05_trait_objects.md`](./hard/05_trait_objects.md)

## Brutal

1. [`01_unsafe_rust.md`](./brutal/01_unsafe_rust.md)
2. [`02_ffi.md`](./brutal/02_ffi.md)
3. [`03_atomics_memory_ordering.md`](./brutal/03_atomics_memory_ordering.md)
4. [`04_const_generics.md`](./brutal/04_const_generics.md)

---

## Installation (Quick)

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source "$HOME/.cargo/env"
rustup update
rustup component add rustfmt clippy
rustc --version
cargo --version
```
