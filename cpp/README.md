# Modern C++ Tutorial (C++17/20/23)

A pragmatic, example-driven guide to C++ language features and the Standard Library. Focuses on modern, safe, and efficient idioms.

- Audience: Developers comfortable with another language, new or returning to C++.
- Dialect: Emphasizes C++17 and C++20, notes C++23 where useful.

## Quick start

```bash
# Compile a single file
g++ -std=c++20 -O2 -Wall -Wextra -pedantic main.cpp -o app

# With Clang
clang++ -std=c++20 -O2 -Wall -Wextra -pedantic main.cpp -o app

# Enable sanitizers (debugging)
g++ -std=c++20 -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp -o app

# Minimal CMake project
cat > CMakeLists.txt <<'CM'
cmake_minimum_required(VERSION 3.20)
project(hello LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_executable(hello main.cpp)
CM

cmake -S . -B build && cmake --build build -j
```

## First program

```cpp
#include <iostream>
#include <string>

int main() {
    std::string name;
    std::cout << "Name: ";
    if (std::getline(std::cin, name)) {
        std::cout << "Hello, " << name << "!\n";
    }
    return 0; // optional in main
}
```

## Core language essentials

- Types: `bool`, integer types, `char`, floating types, `std::string`, `auto`, `enum class`.
- Pointers vs references: raw pointers own nothing; prefer references when non-null and required; prefer smart pointers for ownership.
- Value categories: lvalues vs rvalues; move semantics enable efficient transfers.
- RAII: acquire in constructor, release in destructor. Use objects to manage resources.

### Variables and initialization

Prefer brace initialization to avoid narrowing:

```cpp
int a{42};
double x{3.14};
std::string s{"text"};
```

### Functions and overloading

```cpp
#include <string>

int add(int a, int b) { return a + b; }
std::string add(const std::string& a, const std::string& b) { return a + b; }
```

Default arguments belong in declarations:

```cpp
int connect(std::string host, int port = 80);
```

### Inline, constexpr, consteval, constinit

```cpp
constexpr int square(int x) { return x * x; }
consteval int always_compile_time(int x) { return x * x; } // C++20
constinit int global_counter = 0; // Initialized at compile-time, not const
```

### Templates and type deduction

```cpp
#include <type_traits>

template <typename T>
T max_value(const T& a, const T& b) { return (a < b) ? b : a; }

// Constrained (C++20 concepts)
#include <concepts>

template <std::totally_ordered T>
T min_value(const T& a, const T& b) { return (b < a) ? b : a; }
```

### Classes, constructors, and RAII

```cpp
#include <cstdio>
#include <stdexcept>

class File {
public:
    explicit File(const char* path, const char* mode) : handle(std::fopen(path, mode)) {
        if (!handle) throw std::runtime_error("open failed");
    }

    // Non-copyable, movable
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
    File& operator=(File&& other) noexcept {
        if (this != &other) { close(); handle = other.handle; other.handle = nullptr; }
        return *this;
    }

    ~File() { close(); }

    std::FILE* get() const { return handle; }

private:
    std::FILE* handle{};
    void close() { if (handle) { std::fclose(handle); handle = nullptr; } }
};
```

### Inheritance and polymorphism

```cpp
#include <iostream>

struct Shape { virtual ~Shape() = default; virtual double area() const = 0; };
struct Rect : Shape { double w{}, h{}; double area() const override { return w * h; } };

void print_area(const Shape& s) { std::cout << s.area() << '\n'; }
```

Use interfaces sparingly; prefer composition. If polymorphism is needed, consider type-erasure or `std::variant`.

### Enums

```cpp
enum class Color { Red, Green, Blue };
Color c = Color::Red;
```

### Lambdas

```cpp
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> v{1,2,3,4,5};
    int threshold = 3;
    auto count = std::count_if(v.begin(), v.end(), [threshold](int x){ return x > threshold; });
}
```

Captures: `[=]` by value, `[&]` by reference, `[this]`, and explicit lists. Use `mutable` to modify captured-by-value copies.

### Structured bindings and `auto`

```cpp
#include <tuple>

std::tuple<int, double, const char*> t{1, 3.14, "pi"};
auto [i, d, s] = t; // C++17
```

### Ranges (C++20)

```cpp
#include <ranges>
#include <vector>
#include <algorithm>

std::vector<int> v{1,2,3,4,5,6};
auto odds = v | std::views::filter([](int x){ return x % 2; })
               | std::views::transform([](int x){ return x * x; });
for (int x : odds) { /* ... */ }
```

### Move semantics and forwarding

```cpp
#include <utility>
#include <string>

struct Buffer {
    std::string data;
    Buffer() = default;
    explicit Buffer(std::string d) : data(std::move(d)) {}

    Buffer(const Buffer&) = default;            // copy
    Buffer& operator=(const Buffer&) = default;
    Buffer(Buffer&&) noexcept = default;        // move
    Buffer& operator=(Buffer&&) noexcept = default;
};

void sink(Buffer b);

template <typename T>
void wrapper(T&& t) { sink(std::forward<T>(t)); }
```

### Exceptions and error handling

- Prefer exceptions for exceptional, non-local errors. Mark functions `noexcept` when they do not throw.
- For recoverable errors or optional values, prefer `std::optional` or `std::expected` (C++23; or `tl::expected`).

```cpp
#include <optional>

std::optional<int> parse_int(const std::string& s);
```

## Smart pointers and ownership

- `std::unique_ptr<T>`: sole ownership, cheap move, non-copyable.
- `std::shared_ptr<T>`: shared ownership, atomic refcount.
- `std::weak_ptr<T>`: non-owning observation of `shared_ptr`.

```cpp
#include <memory>

struct Widget { /* ... */ };

std::unique_ptr<Widget> make_widget() {
    return std::make_unique<Widget>();
}
```

Prefer value or `unique_ptr` members; avoid `shared_ptr` unless required.

## Containers overview

- Sequence containers: `std::array`, `std::vector`, `std::deque`, `std::list`, `std::forward_list`.
- Strings: `std::string`, `std::string_view` (non-owning view), `std::u8/u16/u32string` for Unicode encodings.
- Associative: `std::map`, `std::multimap`, `std::set`, `std::multiset`.
- Unordered: `std::unordered_map`, `std::unordered_set`, and their multi variants.
- Adapters: `std::stack`, `std::queue`, `std::priority_queue`.

### `std::vector` basics

```cpp
#include <vector>
#include <algorithm>

std::vector<int> v;
v.reserve(100);
v.push_back(42);
std::sort(v.begin(), v.end());
```

### `std::string` and `std::string_view`

```cpp
#include <string>
#include <string_view>

std::string s = "hello";
std::string_view view = s; // view does not own, must not outlive s
```

### `std::unordered_map`

```cpp
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> counts;
counts["alice"]++;
if (auto it = counts.find("bob"); it != counts.end()) {
    // it->second
}
```

## Algorithms and iterators

- Core algorithms: `sort`, `find`, `transform`, `accumulate`, `copy`, `remove_if`, `partition`, etc.
- Non-modifying vs modifying algorithms; use iterator ranges `[first, last)`.
- C++20 Ranges provide range-based overloads and pipelines.

```cpp
#include <algorithm>
#include <numeric>
#include <vector>

std::vector<int> v{1,2,3,4,5};
int sum = std::accumulate(v.begin(), v.end(), 0);
std::transform(v.begin(), v.end(), v.begin(), [](int x){ return x * 2; });
```

### Parallel algorithms (C++17)

```cpp
#include <algorithm>
#include <execution>
#include <vector>

std::vector<double> v(1'000'000);
std::for_each(std::execution::par, v.begin(), v.end(), [](double& x){ x = std::sqrt(1 + x); });
```

## Utilities

- `std::optional<T>`: optional values.
- `std::variant<Ts...>`: type-safe tagged union.
- `std::any`: type-erased container.
- `std::pair`, `std::tuple`, `std::tie`/structured bindings.
- `std::chrono`: time points, durations, clocks.
- `std::filesystem`: paths, files, directories.
- `std::format` (C++20): safe formatting; use `fmt` library if unavailable.
- `std::span<T>` (C++20): non-owning view over contiguous sequence.
- `<random>`: RNG engines and distributions.
- `<bit>` (C++20): bit operations like `std::popcount`, `std::rotl`.

### `std::optional` and `std::variant`

```cpp
#include <optional>
#include <variant>
#include <string>

std::optional<int> maybe_index(bool ok) { return ok ? std::optional{42} : std::nullopt; }

using Result = std::variant<int, std::string>;

int use(Result r) {
    return std::visit([](auto&& value) -> int {
        using V = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<V, int>) return value;
        else return static_cast<int>(value.size());
    }, r);
}
```

### `std::filesystem`

```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

for (const auto& entry : fs::directory_iterator(".")) {
    std::cout << entry.path() << '\n';
}
```

### Formatting

```cpp
#include <format>
#include <string>

std::string s = std::format("{} + {} = {}", 2, 3, 5);
```

If `<format>` is unavailable, use the `fmt` library (`<fmt/core.h>`, `fmt::format`).

### Random numbers

```cpp
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 6);
int roll = dist(rng);
```

## Concurrency

- Threads: `std::thread`.
- Synchronization: `std::mutex`, `std::lock_guard`, `std::unique_lock`, `std::condition_variable`.
- Tasks: `std::async`, `std::future`, `std::promise`.
- Atomics: `std::atomic<T>`.
- Parallel algorithms: see above.

```cpp
#include <thread>
#include <mutex>
#include <vector>

std::mutex m;
int shared_value = 0;

void worker() {
    std::lock_guard<std::mutex> lock(m);
    ++shared_value;
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) threads.emplace_back(worker);
    for (auto& t : threads) t.join();
}
```

C++20 coroutines (`co_await`) exist but require a library layer; consider libraries like cppcoro or async runtimes.

## Error handling patterns

- Use exceptions for unexpected failures; propagate with stack unwinding.
- Use `std::expected<T, E>` (C++23) or `tl::expected` to model recoverable outcomes.
- Avoid raw `new`/`delete`; avoid manual `try/catch` unless you can handle or add context.
- Prefer `noexcept` where strong guarantees exist.

## I/O

- `<iostream>`: stream-based I/O; consider performance.
- `<format>`: modern formatting output.
- `<cstdio>`: C stdio when interop or raw speed needed.

```cpp
#include <iostream>
#include <iomanip>

double x = 3.14159;
std::cout << std::fixed << std::setprecision(2) << x << '\n';
```

## Guidelines and best practices

- Prefer standard containers and algorithms; avoid reinventing data structures.
- Rule of Zero/Five: prefer types that do not manage resources; if they do, implement move/copy appropriately.
- `const` correctness: mark what does not change; prefer passing by `const&` or by value when cheap.
- Avoid macros; use `constexpr`, templates, and inline functions.
- Keep headers minimal: use forward declarations, avoid including heavy headers in public headers; use `pimpl` if needed.
- Use `= default` and `= delete` to control special member functions.
- Minimize global state; prefer function-local `static` when needed (thread-safe in C++11+).
- Prefer `enum class` over plain enums.
- Avoid `dynamic_cast`; design with virtual interfaces or variants.
- Use tools: sanitizers (ASan/UBSan/TSan), static analyzers (clang-tidy), linters (cpplint), formatters (clang-format).

## Build and tooling

- Compilers: GCC, Clang, MSVC. Test with at least two for portability.
- CMake is de facto build system.
- Enable warnings-as-errors in CI for libraries/apps you control.

```cmake
# .clang-tidy example (YAML)
# Checks: '-*,modernize-*,performance-*,bugprone-*'
```

## Testing

Popular frameworks:

- GoogleTest (`gtest`)
- Catch2
- doctest

Example (Catch2):

```cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

int add(int a, int b) { return a + b; }

TEST_CASE("addition") { REQUIRE(add(2, 3) == 5); }
```

## Project structure (small library)

```
project/
  include/project/thing.hpp
  src/thing.cpp
  tests/
  CMakeLists.txt
```

`thing.hpp` (public API) should not include heavy dependencies; `thing.cpp` includes what it needs.

## Performance notes

- Measure first; do not guess. Use `chrono`, profilers, and benchmarks (Google Benchmark).
- Reserve capacity for vectors when size is known.
- Use move semantics to avoid copies.
- Consider `small_vector`-like types (e.g., `llvm::SmallVector`) when appropriate, but default to `std` first.
- Avoid needless virtual dispatch; prefer static polymorphism (templates) when performance-critical.

## Handy recipes

- Read whole file:

```cpp
#include <fstream>
#include <iterator>
#include <string>

std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}
```

- Split string (simple):

```cpp
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string_view> split(std::string_view s, char delim) {
    std::vector<std::string_view> parts;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delim, start);
        if (pos == std::string_view::npos) { parts.emplace_back(s.substr(start)); break; }
        parts.emplace_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return parts;
}
```

- Timed scope (RAII):

```cpp
#include <chrono>
#include <iostream>

struct ScopedTimer {
    std::chrono::steady_clock::time_point start{std::chrono::steady_clock::now()};
    ~ScopedTimer() {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        std::cout << ms << " ms\n";
    }
};
```

## Further learning

- C++ Core Guidelines: `https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines`
- cppreference: `https://en.cppreference.com`
- ISO C++ committee papers index: `https://wg21.link`
- Jason Turner's C++ Weekly: `https://www.youtube.com/c/lefticus1`
- "Effective Modern C++" (Meyers), "A Tour of C++" (Stroustrup)

---

If something here doesn't compile with your toolchain, try `-std=c++20` or upgrade your compiler. Some sections (e.g., `std::format`, `std::expected`) may require newer standard library versions.