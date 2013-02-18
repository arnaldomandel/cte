CFLAGS= -Wall -pedantic

all: ../bin/cte

../bin/cte: main.o messages.o read_file.o
	cc $(CFLAGS) -o $@ $^

main.o: main.c messages.h read_file.h

messages.o: messages.h messages.c

read_file.o: read_file.h messages.h read_file.c

clean:
	rm -f *.o
	rm -f cte
	rm -f test/*.o

#Tests
test/assert.o: test/assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

test/tree_test.o: test/tree_test.c 
	cc $(CFLAGS) -I . -c -o $@ $^

tree_test: ../bin/tree_test

../bin/tree_test: test/tree_test.o tree.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

test/bic_test.o: test/bic_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

bic_test: ../bin/bic_test

../bin/bic_test: test/bic_test.o tree.o bic_setup.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^
