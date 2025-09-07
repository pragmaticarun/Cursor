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
