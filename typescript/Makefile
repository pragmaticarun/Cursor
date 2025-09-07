NPM=npm
TS=node node_modules/typescript/bin/tsc

.PHONY: build test format examples run

build:
	$(NPM) run build --silent

run: build
	node dist/main.js

examples: build
	npx ts-node --compiler-options '{"module":"esnext"}' examples/advanced_types.ts || node dist/examples/advanced_types.js || true

test:
	$(NPM) test --silent

format:
	$(NPM) run format --silent