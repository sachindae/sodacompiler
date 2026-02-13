SRC = src/lexer.c src/parser.c
TEST_SRC = test/test_lexer.c

compiler: $(SRC) src/main.c
	gcc -o scc $(SRC) src/main.c

debug: $(SRC) src/main.c
	gcc -g -o scc $(SRC) src/main.c

tests: $(TEST_SRC)
	gcc -o tests $(SRC) $(TEST_SRC)

clean:
	rm -f scc tests *.o
