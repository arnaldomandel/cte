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
tests:	../bin/tree_test


test/tree_test.o: test/tree_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/tree_test: test/tree_test.o tree.o
	cc $(CFLAGS) -I . -o  $^
