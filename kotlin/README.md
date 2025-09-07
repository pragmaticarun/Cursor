# Kotlin Tutorial: Language Essentials and Standard Library (JVM)

## Quick start

```bash
# Using kotlinc if installed
kotlinc Main.kt -include-runtime -d app.jar && java -jar app.jar
```

## Language basics

- `val` vs `var`, nullable types, Elvis `?:`
- Data classes, sealed classes
- Extension functions
- Coroutines (with kotlinx.coroutines)

## Standard library highlights

- Collections, sequences
- `java.time` interop
- File I/O with `java.nio.file`

See `Main.kt` for a runnable demo.