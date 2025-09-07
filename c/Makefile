CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2
SRC=main.c
BIN=app

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
	rm -f $(BIN)