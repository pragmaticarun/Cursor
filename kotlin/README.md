# Kotlin Tutorial: Language Essentials and Standard Library (JVM)

## Quick start

```bash
kotlinc kotlin/Main.kt -include-runtime -d kotlin/app.jar && java -jar kotlin/app.jar
```

## Language basics

- Null-safety, `?`, `!!`, Elvis `?:`, `let`, `also`, `apply`
- Data classes, sealed classes, object singletons, companion objects
- Extension functions/properties, infix functions
- Coroutines (kotlinx.coroutines): `launch`, `async`, `flow`

## Standard library highlights

- Collections and sequences; mapping, filtering, grouping
- `Result`, `runCatching`
- Interop with Java (`java.time`, `Files`)

See `kotlin/Main.kt` for runnable examples.