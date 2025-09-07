NODE=node

.PHONY: run test format examples

run:
	$(NODE) index.js

test:
	npm test --silent

format:
	npm run format --silent

examples:
	$(NODE) examples/iterators.js
	$(NODE) examples/streams.js