#!/usr/bin/env python3
from __future__ import annotations

import asyncio
import json
import re
from collections import Counter, defaultdict
from dataclasses import dataclass
from datetime import datetime, timezone
from functools import lru_cache
from pathlib import Path
from typing import Optional, Iterable


@dataclass(frozen=True)
class Person:
    id: int
    name: str
    email: Optional[str] = None


def word_frequencies(text: str) -> dict[str, int]:
    words = re.findall(r"[a-zA-Z]+", text.lower())
    return Counter(words)


def list_python_files(directory: Path) -> list[Path]:
    return [p for p in directory.rglob("*.py") if p.is_file()]


@lru_cache(maxsize=None)
def fibonacci(n: int) -> int:
    if n < 2:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)


async def async_sum(values: Iterable[int]) -> int:
    await asyncio.sleep(0)  # simulate async work
    return sum(values)


def main() -> None:
    print("=== Python demo ===")

    # Dataclass usage
    people = [
        Person(1, "Ada"),
        Person(2, "Alan", email="alan@example.com"),
    ]
    print("People:", people)

    # Collections and comprehensions
    squares = {n: n * n for n in range(6)}
    print("Squares:", squares)

    nested = defaultdict(list)
    nested["evens"].extend([n for n in range(10) if n % 2 == 0])
    print("Nested:", dict(nested))

    # Regex + Counter
    text = "Hello hello World! world of code"
    print("Word frequencies:", word_frequencies(text))

    # Filesystem
    here = Path(__file__).resolve().parent
    print("This folder:", here)
    print("Python files in repo root (sample):", list_python_files(here.parent)[:5])

    # Caching
    print("Fibonacci(10):", fibonacci(10))

    # Datetime
    print("Now UTC:", datetime.now(timezone.utc).isoformat())

    # JSON
    blob = {"people": [p.__dict__ for p in people]}
    print("JSON:", json.dumps(blob, indent=2))

    # Async
    total = asyncio.run(async_sum(range(10)))
    print("Async sum 0..9 =", total)


if __name__ == "__main__":
    main()