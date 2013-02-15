CFLAGS= -Wall -pedantic

all: ../bin/cte

../bin/cte: main.o messages.o read_file.o
	cc $(CFLAGS) -o $@ $^

main.o: main.c messages.h read_file.h

messages.o: messages.h

read_file.o: read_file.h messages.h

clean:
	rm -f *.o
	rm -f cte
	rm -f test/*.o

#Tests
test/assert.o: test/assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

test/tree_test.o: test/tree_test.c 
	cc $(CFLAGS) -I . -c -o $@ $^

tree_test: test/tree_test.o tree.o test/assert.o
	cc $(CFLAGS) -I . -o ../bin/$@ $^

test/bic_test.o: test/bic_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

bic_test: test/bic_test.o tree.o bic.o test/assert.o
	cc $(CFLAGS) -I . -o ../bin/$@ $^
