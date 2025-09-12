# Ordered Map Implementation Makefile
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -Iinclude -I../include
DEBUG_CFLAGS=-std=c11 -Wall -Wextra -g -O0 -Iinclude -I../include -DDEBUG
SANITIZER_CFLAGS=-std=c11 -Wall -Wextra -g -O2 -Iinclude -I../include -fsanitize=address -fsanitize=undefined
SRCDIR=src
TESTDIR=tests
INCDIR=include
BINDIR=bin
EXAMPLEDIR=examples

# Ordered map sources
ORDERED_MAP_SOURCES=$(SRCDIR)/ordered_map.c
ORDERED_MAP_BENCHMARK=$(SRCDIR)/ordered_map_benchmark.c
ORDERED_MAP_EXAMPLE=$(EXAMPLEDIR)/ordered_map_example.c
ORDERED_MAP_TEST=$(TESTDIR)/test_ordered_map.c

# Binaries
ORDERED_MAP_BINARY=$(BINDIR)/ordered_map_example
ORDERED_MAP_BENCHMARK_BINARY=$(BINDIR)/ordered_map_benchmark
ORDERED_MAP_TEST_BINARY=$(BINDIR)/test_ordered_map

# Test framework (from parent directory)
TEST_FRAMEWORK=../src/test_framework.c

# Default target
.PHONY: all example test benchmark clean help

all: example test benchmark

# Build ordered map example
$(ORDERED_MAP_BINARY): $(ORDERED_MAP_EXAMPLE) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build ordered map benchmark
$(ORDERED_MAP_BENCHMARK_BINARY): $(ORDERED_MAP_BENCHMARK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Build ordered map test
$(ORDERED_MAP_TEST_BINARY): $(ORDERED_MAP_TEST) $(TEST_FRAMEWORK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# Create directories
$(BINDIR):
	mkdir -p $(BINDIR)

# Run targets
example: $(ORDERED_MAP_BINARY)
	./$(ORDERED_MAP_BINARY)

test: $(ORDERED_MAP_TEST_BINARY)
	./$(ORDERED_MAP_TEST_BINARY)

benchmark: $(ORDERED_MAP_BENCHMARK_BINARY)
	./$(ORDERED_MAP_BENCHMARK_BINARY)

# Debug versions
debug-example: $(ORDERED_MAP_EXAMPLE) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(DEBUG_CFLAGS) $^ -o $(BINDIR)/debug_ordered_map_example

debug-test: $(ORDERED_MAP_TEST) $(TEST_FRAMEWORK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(DEBUG_CFLAGS) $^ -o $(BINDIR)/debug_test_ordered_map

debug-benchmark: $(ORDERED_MAP_BENCHMARK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(DEBUG_CFLAGS) $^ -o $(BINDIR)/debug_ordered_map_benchmark -lm

# Sanitizer versions
sanitizer-example: $(ORDERED_MAP_EXAMPLE) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(SANITIZER_CFLAGS) $^ -o $(BINDIR)/san_ordered_map_example

sanitizer-test: $(ORDERED_MAP_TEST) $(TEST_FRAMEWORK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(SANITIZER_CFLAGS) $^ -o $(BINDIR)/san_test_ordered_map

sanitizer-benchmark: $(ORDERED_MAP_BENCHMARK) $(ORDERED_MAP_SOURCES) | $(BINDIR)
	$(CC) $(SANITIZER_CFLAGS) $^ -o $(BINDIR)/san_ordered_map_benchmark -lm

# Run sanitizer versions
run-sanitizer-example: sanitizer-example
	./$(BINDIR)/san_ordered_map_example

run-sanitizer-test: sanitizer-test
	./$(BINDIR)/san_test_ordered_map

run-sanitizer-benchmark: sanitizer-benchmark
	./$(BINDIR)/san_ordered_map_benchmark

# Clean build files
clean:
	rm -rf $(BINDIR)
	@echo "Clean complete."

# Help target
help:
	@echo "Ordered Map Implementation Makefile"
	@echo "==================================="
	@echo ""
	@echo "Basic targets:"
	@echo "  all          - Build example, test, and benchmark"
	@echo "  example      - Build and run example"
	@echo "  test         - Build and run tests"
	@echo "  benchmark    - Build and run benchmarks"
	@echo ""
	@echo "Debug targets:"
	@echo "  debug-example    - Build debug version of example"
	@echo "  debug-test       - Build debug version of test"
	@echo "  debug-benchmark  - Build debug version of benchmark"
	@echo ""
	@echo "Sanitizer targets:"
	@echo "  sanitizer-example    - Build sanitizer version of example"
	@echo "  sanitizer-test       - Build sanitizer version of test"
	@echo "  sanitizer-benchmark  - Build sanitizer version of benchmark"
	@echo "  run-sanitizer-*      - Build and run sanitizer versions"
	@echo ""
	@echo "Utility targets:"
	@echo "  clean        - Remove all generated files"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make example     # Run the example"
	@echo "  make test        # Run all tests"
	@echo "  make benchmark   # Run performance benchmarks"
	@echo "  make clean       # Clean build files"
