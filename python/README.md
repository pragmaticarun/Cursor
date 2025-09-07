# Python Tutorial: Language Essentials and Standard Library

A concise, modern overview to get productive with Python 3.11+.

## Quick start

```bash
python3 --version
python3 main.py
```

## Language basics

- **Primitives**: `int`, `float`, `bool`, `str`, `bytes`, `None`
- **Collections**: `list`, `tuple`, `dict`, `set`
- **Control flow**: `if/elif/else`, `for`, `while`, `try/except/finally`, `with`
- **Functions**: default/keyword args, `*args`, `**kwargs`, closures, decorators
- **Comprehensions**: list, dict, set, generator expressions
- **Classes**: dataclasses, properties, dunder methods, context managers (`__enter__/__exit__`)
- **Typing**: `typing` module (`TypedDict`, `Protocol`, generics, `Union`, `Optional`)
- **Iterables**: iterators, generators (`yield`), `itertools`

## Standard library highlights

- **pathlib**: filesystem paths
- **datetime/zoneinfo**: dates and times
- **collections**: `Counter`, `defaultdict`, `deque`, `namedtuple`
- **functools**: `lru_cache`, `partial`, `singledispatch`
- **itertools**: combinatorics and iterator utilities
- **json**: serialize/deserialize
- **re**: regular expressions
- **asyncio**: async I/O

## Example snippets

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
counts = Counter("abracadabra")
print(counts.most_common(2))
```

```python
import itertools as it
pairs = list(it.product([1,2], ["a","b"]))
print(pairs)
```

See `main.py` for a runnable demo using many of these features.