PY=python3
PIP=pip3

.PHONY: run test format install examples

run:
	$(PY) main.py

test:
	$(PY) -m pytest -q

format:
	$(PY) -m isort .
	$(PY) -m black .

install:
	$(PIP) install -r requirements.txt

examples:
	$(PY) examples/pattern_matching.py
	$(PY) examples/dataclasses.py
	$(PY) examples/asyncio_taskgroup.py
	$(PY) examples/pathlib_fs.py
	$(PY) examples/functools_singledispatch.py