CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2
SRC=main.c
BIN=app
EXAMPLE_BIN=examples/app_examples

.PHONY: run test format clean

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

test: $(BIN)
	./$(BIN) >/dev/null

format:
	clang-format -i *.c *.h 2>/dev/null || true

clean:
	rm -f $(BIN) $(EXAMPLE_BIN)

examples: examples/qsort_bsearch.c
	$(CC) $(CFLAGS) examples/qsort_bsearch.c -o $(EXAMPLE_BIN)
	./$(EXAMPLE_BIN) >/dev/null