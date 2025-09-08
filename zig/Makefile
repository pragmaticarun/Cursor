ZIG?=$(CURDIR)/.tools/zig/zig

.PHONY: ensure-zig
ensure-zig:
	@if ! command -v $(ZIG) >/dev/null 2>&1; then \
		echo "Bootstrapping Zig..." ; \
		./scripts/bootstrap_zig.sh ; \
	fi

.PHONY: build run-all test fmt clean list

build: ensure-zig
	$(ZIG) build

run-all: ensure-zig
	$(ZIG) build run-all

test: ensure-zig
	$(ZIG) build test

fmt: ensure-zig
	$(ZIG) fmt .

clean:
	rm -rf zig-out .zig-cache

list: ensure-zig
	$(ZIG) build -l | cat

