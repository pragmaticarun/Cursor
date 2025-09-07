# Ruby Tutorial: Language Essentials and Core Libraries (Ruby 3.2)

## Quick start

```bash
ruby ruby/main.rb
```

## Language basics

- Objects, modules, classes; mixins with `include`
- Blocks, procs, lambdas; Enumerable
- Symbols, arrays, hashes
- Pattern matching (`case/in`), safe navigation `&.`

## Core libraries

- Time/Date, JSON, File/Dir, Pathname
- Open3 for subprocesses
- Net::HTTP for HTTP

See `ruby/main.rb` for runnable examples.

---

## Advanced topics

- Refinements for scoped monkey-patching
- Fiber scheduler (3.0+) for async I/O
- Concurrency: Ractors (experimental), threads, Mutex
- FFI with `fiddle`; native extensions
- Performance tooling: benchmark-ips, stackprof, memory_profiler

## References

- Ruby docs: `https://ruby-doc.org/`
- Ruby style guide: `https://rubystyle.guide/`