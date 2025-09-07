# Swift Tutorial: Language Essentials and Standard Library (Swift 5.9)

## Quick start

```bash
swiftc swift/main.swift -o swift/app && ./swift/app
```

## Language basics

- Optionals, `guard`/`if let`, optional chaining
- Value types (struct/enum) vs reference types (class)
- Protocols, protocol extensions, generics
- Error handling: `throws`, `rethrows`, `Result`

## Standard library & Foundation

- Collections: `Array`, `Dictionary`, `Set`
- Codable for JSON; `JSONEncoder`/`Decoder`
- Dates and files with Foundation

See `swift/main.swift` for runnable examples.

---

## Advanced topics

- Protocol-oriented programming and default implementations
- Async/await and structured concurrency; `Task`, `TaskGroup`
- Actors for data isolation
- Property wrappers and result builders (DSLs)
- Swift Package Manager basics

## References

- Swift docs: `https://www.swift.org/documentation/`