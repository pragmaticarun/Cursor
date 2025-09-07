# Java Tutorial: Language Essentials and Core Libraries (Java 17+)

## Quick start

```bash
javac java/Main.java && java -cp java Main
```

## Language basics

- Types, generics, var (local type inference)
- Records, sealed classes/interfaces (17+)
- Lambdas and method references; functional interfaces
- Streams API and collectors
- Pattern matching for `instanceof` (17+)
- Exceptions; try-with-resources (AutoCloseable)

## Core libraries

- Collections: `List`, `Map`, `Set`, `Optional`
- Time: `java.time` (`Instant`, `LocalDateTime`, `ZoneId`)
- Filesystem: `java.nio.file` (`Path`, `Files`)
- Concurrency: `CompletableFuture`, executors, locks
- HTTP: `java.net.http.HttpClient` (11+)
- JSON: use Jackson/Gson or `java.json` (EE) for JSON

## Idioms

- Prefer immutable data (records) when appropriate
- Use streams for bulk operations; avoid overcomplicating
- Use `Optional` for absent values, not for fields in entities

See `java/Main.java` for runnable examples.