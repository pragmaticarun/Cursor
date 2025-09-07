# JavaScript Tutorial: Language Essentials and Core APIs (Node 18+)

## Quick start

```bash
node --version
node javascript/index.js
```

## Language basics

- Declarations: `let`, `const`, block scoping vs `var`
- Functions: arrow functions, default/rest params, closures
- Objects/Classes: prototypes, `class`, fields, `this`, private fields `#x`
- Destructuring and spread/rest for arrays/objects
- Template literals, tagged templates
- Iteration: iterables/iterators, `for...of`, generators `function*`
- Modules: ESM `import`/`export`; top-level `await` in ESM
- Async: Promises, `async`/`await`, Promise combinators (`all`, `allSettled`, `race`, `any`)
- Intl: `Intl.NumberFormat`, `Intl.DateTimeFormat`

## Core Node APIs

- File system: `fs/promises`, streams
- Path/URL: `path`, `url`, `URL`
- OS and process: `os`, `process`
- Timers: `timers/promises`
- HTTP(S): `http`, `https`, fetch (Node 18+)
- Diagnostics: `console`, `util.inspect`

## Useful patterns

- Use `Array.prototype` methods (`map`, `filter`, `reduce`, `flatMap`)
- Prefer `Map`/`Set` over plain objects for key-value and membership
- Top-level `await` in ESM for straightforward async scripts
- Use streams for large I/O to avoid buffering entire files

See `javascript/index.js` for a runnable demo of destructuring, arrays, sets/maps, regex, file I/O, and async.