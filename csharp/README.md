# C# Tutorial: Language Essentials and .NET Libraries (.NET 8)

## Quick start

```bash
dotnet run --project csharp
```

## Language basics

- Types and nullable reference types (`string?`)
- Records vs classes; with-expressions; `init` accessors
- Pattern matching: `switch`, `is`, relational patterns
- LINQ: `Select`, `Where`, `Aggregate`, query syntax
- Async/await: `Task`, `ValueTask`, cancellation

## BCL highlights

- Collections: `List<T>`, `Dictionary<TKey,TValue>`, `ImmutableArray<T>` (System.Collections.Immutable)
- IO: `System.IO` (streams, files), `System.IO.Pipelines`
- Text/JSON: `System.Text.RegularExpressions`, `System.Text.Json`
- Parallelism: `System.Threading.Tasks`, `Parallel`, `PLINQ`
- Time: `DateTimeOffset`, `TimeOnly`, `Stopwatch`

## Guidance

- Prefer records for immutable DTOs; classes for behavior
- Use `await using` for async disposables
- Configure `JsonSerializerOptions` for strict casing and null handling

See `csharp/Program.cs` for runnable examples.