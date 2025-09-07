SWIFTC=swiftc
SRC=main.swift
BIN=app

.PHONY: run format clean

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC)
	$(SWIFTC) $(SRC) -o $(BIN)

format:
	swift-format -i $(SRC) 2>/dev/null || true

clean:
	rm -f $(BIN)