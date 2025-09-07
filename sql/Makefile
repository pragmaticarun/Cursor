DB=demo.db

.PHONY: run format clean examples

run:
	sqlite3 $(DB) < example.sql

format:
	command -v sqlfluff >/dev/null 2>&1 && sqlfluff fix . || true

clean:
	rm -f $(DB)

examples: run