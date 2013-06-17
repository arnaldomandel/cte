# Time-stamp: <2013/05/04 22:23:39 hutzpa [hutzpa] am>
CC= c99
CFLAGS= -Wall -g -I. -I.. -L.. -pg  -D_XOPEN_SOURCE=700
LDFLAGS= -lm

allo: assert.o bic_test.o champ_test.o kmp_test.o tree_test.o thresh_test.o maxfreq_test.o

all: ../bin/cte bic_test champ_test kmp_test tree_test thresh_test maxfreq_test 

../../bin/cte: main.o messages.o read_file.o tau.o bic_setup.o bic_calculator.o champion_set.o tree.o resample.o kmp.o bootstrap.oy
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bic_calculator.o: ../bic_calculator.o
bic_setup.o: ../bic_setup.o
champion_set.o: ../champion_set.o
#main.o: main.c read_file.h bic.h tau.h champion_set.h bootstrap.h messages.h
#messages.o: messages.c messages.h
read_file.o: ../read_file.o
resample.o: ../resample.o
tau.o: ../tau.o
tree.o: ../tree.o
draw.o: ../draw.o

clean:
	rm -f *.o
	rm -f test/*.o
	rm -f ../bin/*

#Tests
assert.o: assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

###########
# tree test
###########
tree_test: ../../bin/tree_test

tree_test.o: tree_test.c 
	cc $(CFLAGS) -I . -c -o $@ $^

../../bin/tree_test: tree_test.o tree.o assert.o
	cc $(CFLAGS) -I . -o $@ $^

################
# BIC Setup Test
################
bic_test: bic_test.o tree.o bic_setup.o assert.o bic_calculator.o tau.o

#bic_test.o: bic_test.c
#	$(CC) $(CFLAGS) -o $@ $^ 

../../bin/bic_test: bic_test.o tree.o bic_setup.o assert.o bic_calculator.o tau.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# BIC Calculation test
######################
bic_calculation_test: ../../bin/bic_calculation_test

bic_calculation_test.o: bic_calculation_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

../bin/bic_calculation_test: bic_calculation_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#########
#kmp test
#########
kmp_test: ../bin/kmp_test

kmp_test.o: kmp_test.c
	cc $(CFLAGS) -I . -c -o $@ $^

../../bin/kmp_test: assert.o kmp.o test/kmp_test.o
	cc $(CFLAGS) -I . -o $@ $^

######################
# Champions test
######################
champ_test: ../../bin/champ_test


../../bin/champ_test: champ_test.o tree.o bic_setup.o bic_calculator.o tau.o draw.o read_file.o assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# Threshold test
######################
thresh_test: ../bin/thresh_test


../bin/thresh_test: thresh_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# Max frequency test
######################
maxfreq_test: ../bin/maxfreq_test


../bin/maxfreq_test: maxfreq_test.o tree.o bic_setup.o read_file.o assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

maxfreq_test.o: maxfreq_test.c 
	$(CC) $(CFLAGS) -I . -c -o $@ $^
