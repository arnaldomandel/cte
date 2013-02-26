CFLAGS= -Wall -pedantic -g

all: ../bin/cte bic_calculation_test bic_test kmp_test

../bin/cte: main.o messages.o read_file.o tau.o bic_setup.o bic_calculator.o champion_set.o tree.o
	cc $(CFLAGS) -o $@ $^

bic_calculator.o: bic_calculator.c tau.h tree.h
bic_setup.o: bic_setup.c bic.h tau.h tree.h
champion_set.o: champion_set.c tau.h bic.h champion_set.h
debug.o: debug.c tree.h
main.o: main.c read_file.h bic.h tau.h champion_set.h
messages.o: messages.c messages.h
read_file.o: read_file.c
tau.o: tau.c tau.h
tree.o: tree.c tree.h

clean:
	rm -f *.o
	rm -f test/*.o
	rm -f ../bin/*

#Tests
test/assert.o: test/assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

###########
# tree test
###########
tree_test: ../bin/tree_test

test/tree_test.o: test/tree_test.c 
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/tree_test: test/tree_test.o tree.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

################
# BIC Setup Test
################
bic_test: ../bin/bic_test

test/bic_test.o: test/bic_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/bic_test: test/bic_test.o tree.o bic_setup.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

######################
# BIC Calculation test
######################
bic_calculation_test: ../bin/bic_calculation_test

test/bic_calculation_test.o: test/bic_calculation_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/bic_calculation_test: test/bic_calculation_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o debug.o
	cc $(CFLAGS) -I . -o $@ $^

#########
#kmp test
#########
kmp_test: ../bin/kmp_test

test/kmp_test.o: test/kmp_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../bin/kmp_test: test/assert.o kmp.o test/kmp_test.o
	cc $(CFLAGS) -I . -o $@ $^
