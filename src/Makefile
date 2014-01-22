# Time-stamp: <2014/01/22 16:33:00 benavuya.ime.usp.br [benavuya] am>
CC= c99
#ifdef graph
#	RTL=-fdump-rtl-expand
#endif
CFLAGS= -Wall -g -pg -D_XOPEN_SOURCE=700 $(RTL)
LDFLAGS= -lm #-lgsl -lgslcblas #-lalglib -lstdc++
#LDFLAGS= -lm -lalglib -lstdc++

all: ../bin/cte ../bin/champ_test ../bin/simula ../bin/generate TAGS

# generate call graph as call.pdf
# requires egypt and dot (graphviz)
graph:
	make clean
	make RTL=-fdump-rtl-expand
	egypt *.expand | dot -Tpdf -o call.pdf

tests:  bic_test kmp_test maxfreq_test
	$(MAKE) -C test allo

TAGS:
	etags -o TAGS *.[hc]

../bin/cte: main.o messages.o read_file.o tau.o bic_setup.o bic_calculator.o champion_set.o tree.o resample.o bootstrap.o draw.o glob.o #stats.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

../bin/simula: simula.o read_file.o tau.o tree.o glob.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

../bin/generate: generate.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bic_calculator.o: bic_calculator.c tau.h tree.h glob.h
bic_setup.o: bic_setup.c bic.h tau.h tree.h glob.h
champion_set.o: champion_set.c tau.h bic.h champion_set.h glob.h
main.o: main.c read_file.h bic.h tau.h champion_set.h bootstrap.h messages.h glob.h
messages.o: messages.c messages.h glob.h
read_file.o: read_file.c glob.h
resample.o: resample.c bic.h messages.h glob.h
bootstrap.o: bic.h tau.h champion_set.h bootstrap.h glob.h #stats.h
tau.o: tau.c tau.h glob.h 
tree.o: tree.c tree.h glob.h
vec.o: vec.c vec.h glob.h
student.o: student.cpp  glob.h
draw.o: draw.c glob.h tau.h tree.h
glob.o: glob.c glob.h
simula.o: simula.c glob.h tree.h tau.h read_file.h
generate.o: glob.h params.h
#stats.o: stats.c stats.h

# alglib
specialfunctions.o: alglib/specialfunctions.cpp
	g++    -c -o $@ $^
statistics.o: alglib/statistics.cpp
	g++    -c -o $@ $^
ap.o: alglib/ap.cpp
	g++    -c -o $@ $^
alglibmisc.o: alglib/alglibmisc.cpp
	g++    -c -o $@ $^
alglibinternal.o: alglib/alglibinternal.cpp
	g++    -c -o $@ $^

clean:
	rm -f *.o *.expand
	rm -f test/*.o
	rm -f ../bin/*

######################
# Champions test
######################
champ_test.o: champ_test.c glob.h read_file.h bic.h tree.h champion_set.h tau.h draw.h

../bin/champ_test: champ_test.o tree.o bic_setup.o bic_calculator.o tau.o champion_set.o draw.o read_file.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)




#Tests
test/assert.o: test/assert.c
	cc $(CFLAGS) -I . -c -o $@ $^

###########
# tree test
###########
tree_test: ../bin/tree_test

test/tree_test.o: test/tree_test.c
	$(MAKE) -C $(@D) $(@F)
	#cc $(CFLAGS) -I . -c -o $@ $^

../bin/tree_test: test/tree_test.o tree.o test/assert.o
	cc $(CFLAGS) -I . -o $@ $^

################
# BIC Setup Test
################
bic_test: ../bin/bic_test

test/bic_test.o: test/bic_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -o $@ $^ 

../bin/bic_test: test/bic_test.o tree.o bic_setup.o test/assert.o bic_calculator.o tau.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# BIC Calculation test
######################
bic_calculation_test: ../bin/bic_calculation_test

test/bic_calculation_test.o: test/bic_calculation_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

../bin/bic_calculation_test: test/bic_calculation_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#########
#kmp test
#########
kmp_test: ../bin/kmp_test

test/kmp_test.o: test/kmp_test.c
	$(MAKE) -C $(@D) $(@F)
	# cc $(CFLAGS) -I . -c -o $@ $^

../bin/kmp_test: test/assert.o kmp.o test/kmp_test.o
	$(CC) $(CFLAGS) -I . -o $@ $^

######################
# Maxfreq test
######################
maxfreq_test: ../bin/maxfreq_test



test/maxfreq_test.o: test/maxfreq_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -I . -I .. -o $@ $^ $(LDFLAGS)

../bin/maxfreq_test: test/maxfreq_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
