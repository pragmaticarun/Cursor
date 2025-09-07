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

---

## Advanced language features

- Iteration protocols: custom iterables/iterators, `Symbol.iterator` and `Symbol.asyncIterator`
- Generators and async generators (`for await...of`)
- Proxies and `Reflect` for meta-programming
- Symbols as non-colliding keys; well-known symbols (`toStringTag`, `hasInstance`)
- Error handling best-practices with `AggregateError` and causal errors (`new Error('x',{cause:e})`)
- `AbortController`/`AbortSignal` for cancellable async APIs

## Advanced Node APIs

- Streams: object-mode transforms, pipeline/finished, backpressure control
- Buffers and TypedArrays for binary data
- Worker Threads for CPU-bound tasks; `MessageChannel`
- Child processes (`spawn`, `execFile`) and IPC
- Performance: `perf_hooks`, `v8` heap statistics
- Diagnostics: `node --inspect`, `console.time`, `diagnostics_channel`

## Tooling

- Format with Prettier; lint with ESLint
- Test with Vitest/Jest; add coverage with c8/nyc
- Package as ESM, declare exports in `package.json` `exports`

## References

- MDN JS docs: `https://developer.mozilla.org/docs/Web/JavaScript`
- Node API: `https://nodejs.org/api/`