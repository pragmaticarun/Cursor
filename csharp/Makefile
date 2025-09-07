DOTNET=dotnet

.PHONY: run test format restore

restore:
	$(DOTNET) restore

run:
	$(DOTNET) run --project .

test:
	@echo "Add xUnit/NUnit for real tests; smoke running Program"
	$(DOTNET) build

format:
	$(DOTNET) format || true