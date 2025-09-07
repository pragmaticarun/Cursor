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

---

## Advanced language features

- Sealed hierarchies + pattern matching (preview in newer JDKs)
- Switch expressions and pattern matching for switch (17+/preview)
- Text blocks for multiline strings
- Virtual threads (Project Loom, Java 21) for lightweight concurrency
- Module system (JPMS) for strong encapsulation

## Advanced JDK features

- Streams: custom collectors, parallel streams with common pitfalls
- Concurrency: `CompletableFuture` composition, timeouts, `StructuredTaskScope` (21)
- Reactive: `Flow` API (Reactive Streams)
- NIO: `ByteBuffer`, `FileChannel`, memory-mapped files
- Foreign memory (Panama, incubating) for off-heap access

## Tooling

- Build: Maven/Gradle; Testing: JUnit 5; Formatting: google-java-format/spotless
- Benchmark with JMH; Profile with JFR/async-profiler

## References

- OpenJDK docs: `https://docs.oracle.com/en/java/`
- Java Tutorials: `https://dev.java/learn/`