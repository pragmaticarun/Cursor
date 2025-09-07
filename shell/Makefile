SHELL:=/usr/bin/bash

.PHONY: run test format examples

run:
	bash run.sh

test:
	command -v shellcheck >/dev/null 2>&1 && shellcheck run.sh || true

format:
	command -v shfmt >/dev/null 2>&1 && shfmt -w . || true

examples:
	bash run.sh world