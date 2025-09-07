JAVAC=javac
JAVA=java
SRC=Main.java
BIN=.
EXAMPLES=examples

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

examples:
	mkdir -p $(EXAMPLES)
	printf "%s\n" 'public class ExampleStreams { public static void main(String[] a){ System.out.println(java.util.List.of(1,2,3).stream().map(x->x*x).toList()); }}' > $(EXAMPLES)/ExampleStreams.java
	$(JAVAC) $(EXAMPLES)/ExampleStreams.java
	$(JAVA) -cp .:$(EXAMPLES) ExampleStreams