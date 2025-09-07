# TypeScript Tutorial: Language Features and Standard Library

Type-safe JavaScript with powerful tooling.

## Quick start

```bash
# One-off compile
npx tsc typescript/main.ts --project typescript --outDir typescript/dist
node typescript/dist/main.js
```

## Language basics

- Types: primitives, arrays, tuples, enums, literal types
- Unions/Intersections and type narrowing (control flow analysis)
- Interfaces and type aliases; structural typing
- Generics with constraints and defaults
- Classes and access modifiers; `readonly`; parameter properties
- Modules and namespaces (prefer modules)
- Async/await with typed promises

## Advanced types

- Discriminated unions for variant modeling
- Conditional types and infer: `T extends U ? X : Y`
- Mapped types: `{ [K in keyof T]: ... }`
- Utility types: `Partial`, `Required`, `Pick`, `Omit`, `Record`, `ReturnType`, `Awaited`
- Template literal types and key remapping

## Tooling

- `tsconfig.json` controls target, module, libs, strictness
- Enable `"strict": true` for maximum safety
- Use ESLint and Prettier for linting/formatting

See `typescript/main.ts` for a runnable demo and `typescript/tsconfig.json` for a suggested config.

---

## Advanced language features

- Generic constraints with `extends`, defaults `T = string`, higher-kinded-like patterns via interfaces
- Variadic tuple types, labeled tuple elements
- `satisfies` operator (TS 4.9+) for shape-checking without widening
- `as const` for literal inference and readonly arrays/objects
- `readonly` arrays (`readonly T[]`) and Readonly maps/records
- Decorators (proposed) with `experimentalDecorators` for metadata-based frameworks

## Large project guidance

- Path aliases via `compilerOptions.paths` + Node `--experimental-specifier-resolution` for ESM
- Project references for monorepos; incremental builds
- Public API typing via `types`/`exports` in `package.json`

## References

- TS Handbook: `https://www.typescriptlang.org/docs/handbook/intro.html`