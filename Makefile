SRC = src/lexer.c
TEST_SRC = test/test_lexer.c

compiler: $(SRC) src/main.c
	gcc -o compiler $(SRC) src/main.c

tests: $(TEST_SRC)
	gcc -o tests $(SRC) $(TEST_SRC)

clean:
	rm -f compiler tests *.o
