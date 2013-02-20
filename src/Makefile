CFLAGS= -Wall -pedantic -g

all: ../bin/cte bic_calculation_test bic_test

../bin/cte: main.o messages.o read_file.o
	cc $(CFLAGS) -o $@ $^

main.o: main.c messages.h read_file.h

messages.o: messages.h messages.c

read_file.o: read_file.h messages.h read_file.c

clean:
	rm -f *.o
	rm -f test/*.o
	rm -f ../bin/*

#Tests
test/assert.o: test/assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

# tree test
tree_test: ../bin/tree_test

test/tree_test.o: test/tree_test.c 
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/tree_test: test/tree_test.o tree.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

# BIC Setup Test
bic_test: ../bin/bic_test

test/bic_test.o: test/bic_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/bic_test: test/bic_test.o tree.o bic_setup.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

# BIC Calculation test
bic_calculation_test: ../bin/bic_calculation_test

test/bic_calculation_test.o: test/bic_calculation_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/bic_calculation_test: test/bic_calculation_test.o tree.o bic_setup.o bic_calculator.o tao.o read_file.o test/assert.o debug.o
	cc $(CFLAGS) -I . -o $@ $^
 	