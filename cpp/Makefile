CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O2
SRC=main.cpp
BIN=app

.PHONY: run test format clean

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

test: $(BIN)
	./$(BIN) >/dev/null

format:
	clang-format -i *.cpp *.hpp 2>/dev/null || true

clean:
	rm -f $(BIN)