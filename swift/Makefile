SWIFTC=swiftc
SRC=main.swift
BIN=app
EXAMPLE_SRC=examples/ranges.swift
EXAMPLE_BIN=examples/app

.PHONY: run format clean examples

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC)
	$(SWIFTC) $(SRC) -o $(BIN)

format:
	swift-format -i $(SRC) 2>/dev/null || true

clean:
	rm -f $(BIN)

examples: $(EXAMPLE_SRC)
	$(SWIFTC) $(EXAMPLE_SRC) -o $(EXAMPLE_BIN)
	./$(EXAMPLE_BIN) >/dev/null