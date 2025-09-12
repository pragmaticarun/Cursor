# C Programming Tutorial Makefile
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -Iinclude
DEBUG_CFLAGS=-std=c11 -Wall -Wextra -g -O0 -Iinclude -DDEBUG
SANITIZER_CFLAGS=-std=c11 -Wall -Wextra -g -O2 -Iinclude -fsanitize=address -fsanitize=undefined
SRCDIR=src
TESTDIR=tests
INCDIR=include
BINDIR=bin
OBJDIR=obj
TOOLSDIR=tools
REPORTDIR=reports

# Source files for tutorials (exclude test_framework.c)
TUTORIAL_SOURCES=$(filter-out $(SRCDIR)/test_framework.c,$(wildcard $(SRCDIR)/*.c))
TUTORIAL_BINARIES=$(TUTORIAL_SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%)

# Test sources
TEST_SOURCES=$(wildcard $(TESTDIR)/*.c)
TEST_BINARIES=$(TEST_SOURCES:$(TESTDIR)/%.c=$(BINDIR)/test_%)

# All binaries
ALL_BINARIES=$(TUTORIAL_BINARIES) $(TEST_BINARIES)

# Default target
.PHONY: all tutorials tests run-all test-all format clean help quality advanced

all: tutorials tests

# Build all tutorial programs
tutorials: $(TUTORIAL_BINARIES)

# Build all test programs
tests: $(TEST_BINARIES)

# Build individual tutorial programs
$(BINDIR)/%: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

# Build individual test programs
$(BINDIR)/test_%: $(TESTDIR)/%.c $(SRCDIR)/test_framework.c | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build debug versions
debug: $(TUTORIAL_SOURCES:$(SRCDIR)/%.c=$(BINDIR)/debug_%)

$(BINDIR)/debug_%: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(DEBUG_CFLAGS) $< -o $@

# Build sanitizer versions
sanitizer: $(TUTORIAL_SOURCES:$(SRCDIR)/%.c=$(BINDIR)/san_%)

$(BINDIR)/san_%: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(SANITIZER_CFLAGS) $< -o $@

# Create directories
$(BINDIR):
	mkdir -p $(BINDIR)

# Run all tutorial programs
run-all: tutorials
	@echo "Running all tutorial programs..."
	@for binary in $(TUTORIAL_BINARIES); do \
		echo "Running $$binary:"; \
		./$$binary; \
		echo ""; \
	done

# Run all tests
test-all: tests
	@echo "Running all tests..."
	@for binary in $(TEST_BINARIES); do \
		echo "Running $$binary:"; \
		./$$binary; \
		echo ""; \
	done

# Run individual tutorial
run-%: $(BINDIR)/%
	./$<

# Run individual test
test-%: $(BINDIR)/test_%
	./$<

# Run debug version
debug-%: $(BINDIR)/debug_%
	gdb --batch --ex run --ex quit $<

# Run sanitizer version
san-%: $(BINDIR)/san_%
	./$<

# Format all source files
format:
	@echo "Formatting source files..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(SRCDIR)/*.c $(TESTDIR)/*.c $(INCDIR)/*.h 2>/dev/null || true; \
		echo "Formatting complete."; \
	else \
		echo "clang-format not found, using tools/format_code.sh"; \
		$(TOOLSDIR)/format_code.sh clang; \
	fi

# Advanced formatting
format-advanced:
	@echo "Running advanced formatting..."
	$(TOOLSDIR)/format_code.sh all

# Code quality checks
quality:
	@echo "Running code quality checks..."
	$(TOOLSDIR)/check_quality.sh all

# Quick quality check
quality-quick:
	@echo "Running quick quality check..."
	$(TOOLSDIR)/check_quality.sh compile style bugs

# Memory analysis
memory-check: sanitizer
	@echo "Running memory analysis..."
	@for binary in $(BINDIR)/san_*; do \
		if [ -f "$$binary" ]; then \
			echo "Running $$binary with sanitizer:"; \
			./$$binary; \
			echo ""; \
		fi; \
	done

# Performance profiling
profile: tutorials
	@echo "Running performance profiling..."
	@for binary in $(TUTORIAL_BINARIES); do \
		if [ -f "$$binary" ]; then \
			echo "Profiling $$binary:"; \
			time ./$$binary > /dev/null; \
		fi; \
	done

# Generate documentation
docs:
	@echo "Generating documentation..."
	@mkdir -p $(REPORTDIR)
	@echo "# C Programming Tutorial Documentation" > $(REPORTDIR)/README.md
	@echo "Generated on: $$(date)" >> $(REPORTDIR)/README.md
	@echo "" >> $(REPORTDIR)/README.md
	@echo "## Tutorial Files" >> $(REPORTDIR)/README.md
	@for file in $(TUTORIAL_SOURCES); do \
		basename=$$(basename $$file .c); \
		echo "- $$basename: $$(head -n 3 $$file | tail -n 1 | sed 's/^[ \t]*\*[ \t]*//')" >> $(REPORTDIR)/README.md; \
	done

# Advanced concepts
advanced: $(BINDIR)/09_advanced_types $(BINDIR)/10_best_practices $(BINDIR)/11_debugging_profiling $(BINDIR)/12_performance_optimization
	@echo "Running advanced concepts..."
	@for binary in $(BINDIR)/09_advanced_types $(BINDIR)/10_best_practices $(BINDIR)/11_debugging_profiling $(BINDIR)/12_performance_optimization; do \
		if [ -f "$$binary" ]; then \
			echo "Running $$binary:"; \
			./$$binary; \
			echo ""; \
		fi; \
	done

# Comprehensive example
example: $(BINDIR)/comprehensive
	@echo "Running comprehensive example..."
	./$(BINDIR)/comprehensive

# Build comprehensive example
$(BINDIR)/comprehensive: examples/comprehensive_example.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

# Clean all generated files
clean:
	rm -rf $(BINDIR) $(OBJDIR) $(REPORTDIR)
	@echo "Clean complete."

# Clean reports only
clean-reports:
	rm -rf $(REPORTDIR)
	@echo "Reports cleaned."

# Install development tools
install-tools:
	@echo "Installing development tools..."
	@if command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y clang-format cppcheck valgrind indent astyle; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum install -y clang-tools-extra cppcheck valgrind indent astyle; \
	elif command -v brew >/dev/null 2>&1; then \
		brew install clang-format cppcheck valgrind indent astyle; \
	else \
		echo "Package manager not found. Please install tools manually."; \
	fi

# Help target
help:
	@echo "C Programming Tutorial Makefile"
	@echo "==============================="
	@echo ""
	@echo "Basic targets:"
	@echo "  all          - Build all tutorials and tests"
	@echo "  tutorials    - Build all tutorial programs"
	@echo "  tests        - Build all test programs"
	@echo "  run-all      - Run all tutorial programs"
	@echo "  test-all     - Run all test programs"
	@echo "  run-<name>   - Run specific tutorial (e.g., run-types)"
	@echo "  test-<name>  - Run specific test (e.g., test-types)"
	@echo ""
	@echo "Advanced targets:"
	@echo "  advanced     - Run advanced concepts tutorials"
	@echo "  example      - Run comprehensive example"
	@echo "  debug        - Build debug versions"
	@echo "  sanitizer    - Build sanitizer versions"
	@echo "  debug-<name> - Run debug version with gdb"
	@echo "  san-<name>   - Run sanitizer version"
	@echo ""
	@echo "Quality targets:"
	@echo "  format       - Format source files"
	@echo "  format-advanced - Run advanced formatting"
	@echo "  quality      - Run comprehensive quality checks"
	@echo "  quality-quick - Run quick quality checks"
	@echo "  memory-check - Run memory analysis"
	@echo "  profile      - Run performance profiling"
	@echo ""
	@echo "Utility targets:"
	@echo "  docs         - Generate documentation"
	@echo "  clean        - Remove all generated files"
	@echo "  clean-reports - Remove only reports"
	@echo "  install-tools - Install development tools"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Available tutorials:"
	@echo "  types, control_flow, functions, arrays_strings, pointers,"
	@echo "  stdio, stdlib, strings, advanced_types, best_practices,"
	@echo "  debugging_profiling, performance_optimization"
	@echo ""
	@echo "Ordered Map Implementation:"
	@echo "  See ordered_map/ directory for complete implementation"
	@echo "  cd ordered_map && make help"

# Legacy targets for backward compatibility
run: run-all
test: test-all
$(BINDIR)/app: $(SRCDIR)/main.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@