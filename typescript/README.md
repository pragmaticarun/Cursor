# TypeScript Tutorial: Language Features and Standard Library

Type-safe JavaScript with powerful tooling.

## Quick start

```bash
# Compile without a project (uses defaults)
npx tsc main.ts --target ES2020 --lib ES2020,DOM --outDir dist
node dist/main.js
```

## Language basics

- **Types**: primitives, arrays, tuples, enums, literal types
- **Unions/Intersections** and type narrowing
- **Interfaces** and **type aliases**
- **Generics**
- **Classes** and access modifiers
- **Modules**
- **Async/await** with typed promises

## Utility types

- `Partial`, `Required`, `Pick`, `Omit`, `Record`, `ReturnType`

See `main.ts` for a runnable demo.