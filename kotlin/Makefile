KOTLINC=kotlinc
JAVA=java
SRC=Main.kt
JAR=app.jar

.PHONY: run format clean examples

run: $(JAR)
	$(JAVA) -jar $(JAR)

$(JAR): $(SRC)
	$(KOTLINC) $(SRC) -include-runtime -d $(JAR)

format:
	ktlint -F || true

clean:
	rm -f $(JAR)

examples:
	$(KOTLINC) examples/sequences.kt -include-runtime -d examples/app.jar
	$(JAVA) -jar examples/app.jar >/dev/null