JAVAC=javac
JAVA=java
SRC=Main.java
BIN=.

.PHONY: run test format clean

run: $(SRC)
	$(JAVAC) $(SRC)
	$(JAVA) -cp $(BIN) Main

test:
	@echo "Add JUnit for full testing; running Main as smoke test"
	$(JAVAC) $(SRC)
	$(JAVA) -cp $(BIN) Main >/dev/null

format:
	google-java-format -i *.java 2>/dev/null || true

clean:
	rm -f *.class