KOTLINC=kotlinc
JAVA=java
SRC=Main.kt
JAR=app.jar

.PHONY: run format clean

run: $(JAR)
	$(JAVA) -jar $(JAR)

$(JAR): $(SRC)
	$(KOTLINC) $(SRC) -include-runtime -d $(JAR)

format:
	ktlint -F || true

clean:
	rm -f $(JAR)