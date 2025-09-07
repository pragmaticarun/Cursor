import asyncio
import re
from pathlib import Path

import pytest

from python.main import fibonacci, word_frequencies, async_sum


def test_fibonacci_small():
    assert fibonacci(0) == 0
    assert fibonacci(1) == 1
    assert fibonacci(10) == 55


def test_word_frequencies_basic():
    counts = word_frequencies("Hello hello, world!")
    assert counts["hello"] == 2
    assert counts["world"] == 1


@pytest.mark.asyncio
async def test_async_sum():
    total = await async_sum(range(5))
    assert total == 10