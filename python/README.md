# Python Tutorial: Language Essentials and Standard Library

A concise, modern overview to get productive with Python 3.11+.

## Quick start

```bash
python3 --version
python3 -m venv .venv && source .venv/bin/activate
python3 python/main.py
```

## Language basics

- Primitives: `int`, `float`, `bool`, `str`, `bytes`, `None`
- Collections: `list`, `tuple`, `dict`, `set`, `frozenset`
- Control flow: `if/elif/else`, `for`, `while`, `match` (3.10+), `try/except/else/finally`
- Functions: default/keyword args, `*args`, `**kwargs`, closures, decorators
- Comprehensions: list, dict, set, generator expressions
- Classes: dataclasses, properties, `__repr__`, `__iter__`, context managers (`__enter__/__exit__`)
- Typing: `typing` (`TypedDict`, `Protocol`, generics, `Union`, `Optional`, `Literal`, `Annotated`)
- Iteration: iterators, generators (`yield`), `itertools`
- Concurrency: `asyncio`, `concurrent.futures` (threads/processes)

## Standard library highlights

- Filesystem: `pathlib`, `shutil`, `os`
- Date/time: `datetime`, `zoneinfo`
- Data structures: `collections` (`Counter`, `defaultdict`, `deque`, `namedtuple`)
- Functional: `functools` (`lru_cache`, `partial`, `singledispatch`), `operator`
- Iterators: `itertools` (`product`, `accumulate`, `groupby`)
- Serialization: `json`, `csv`, `pickle` (careful with trust)
- Text: `re`, `textwrap`, `unicodedata`
- Math: `math`, `statistics`, `fractions`, `decimal`
- Utilities: `argparse`, `logging`, `tempfile`, `subprocess`
- Networking: `urllib.request`, `http.server`

## Patterns and idioms

- EAFP over LBYL (use exceptions over pre-checks)
- Context managers for resources (`with open(path) as f:`)
- Prefer `pathlib.Path` over `os.path`
- Prefer `dataclass` for plain-data objects
- Cache expensive pure functions with `@lru_cache`

## Examples

```python
from dataclasses import dataclass
from typing import Optional

@dataclass
class User:
    id: int
    name: str
    email: Optional[str] = None

u = User(1, "Ada")
print(u)
```

```python
from collections import Counter
print(Counter("abracadabra").most_common(3))
```

```python
import itertools as it
print(list(it.groupby(sorted([1,1,2,3,3]))))
```

See `python/main.py` for a runnable demo covering regex, caching, filesystem, datetime, JSON, and asyncio.

---

## Advanced language features

- Pattern matching with `match`/`case` (guards, sequence/dict patterns)
- Descriptors and `@property` for rich attribute behavior
- `__enter__/__exit__` and `contextlib.contextmanager` for custom contexts
- `dataclasses` advanced: `slots=True`, `frozen=True`, `kw_only=True`
- `typing` advanced: `TypeVar`, `ParamSpec`, `TypedDict`, `Protocol`, `Self`, `Final`
- `asyncio` advanced: `gather`, `wait_for`, cancellation, `TaskGroup` (3.11)
- `concurrent.futures` thread/process pools for CPU vs I/O workloads
- Multiprocessing shared data (`multiprocessing.Manager`) and queues

## Useful stdlib cookbook

- `argparse` for CLIs with subcommands and help text
- `logging` with dictConfig, structured logs, and handlers/formatters
- `subprocess.run([...], check=True, capture_output=True, text=True)` for robust child processes
- `tempfile.TemporaryDirectory()` and `NamedTemporaryFile()` for safe temp artifacts
- `pathlib.Path` for atomic replace: `tmp.write_text(...); tmp.replace(target)`
- `zoneinfo.ZoneInfo("UTC")` for timezone-aware datetimes

## Tooling and packaging

- Use `pyproject.toml` (PEP 621) for builds and tool config
- Format with Black, import-sort with isort, lint with Ruff/Flake8, type-check with mypy/pyright
- Test with pytest and `pytest-asyncio` for async tests

## Further reading

- Python docs: `https://docs.python.org/3/`
- `https://docs.python.org/3/library/index.html`
- `https://peps.python.org/`
