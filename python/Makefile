PY=python3
PIP=pip3

.PHONY: run test format install

run:
	$(PY) main.py

test:
	$(PY) -m pytest -q

format:
	$(PY) -m isort .
	$(PY) -m black .

install:
	$(PIP) install -r requirements.txt