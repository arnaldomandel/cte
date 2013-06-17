CC= c99
CFLAGS= -Wall -g -pg -D_XOPEN_SOURCE=700
LDFLAGS= -lalglib -lm -lstdc++

all: ../bin/cte-t bic_test kmp_test champ_test thresh_test maxfreq_test

../bin/cte-t: main.o messages.o read_file.o tau.o bic_setup.o bic_calculator.o champion_set.o tree.o resample.o kmp.o student.o bootstrap.o draw.o
	$(MAKE) -C test allo
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bic_calculator.o: bic_calculator.c tau.h tree.h list.h
bic_setup.o: bic_setup.c bic.h tau.h tree.h list.h
champion_set.o: champion_set.c tau.h bic.h champion_set.h list.h
main.o: main.c read_file.h bic.h tau.h champion_set.h bootstrap.h messages.h list.h
messages.o: messages.c messages.h list.h
read_file.o: read_file.c list.h
resample.o: resample.c bic.h kmp.h messages.h list.h
bootstrap.o: bic.h tau.h champion_set.h bootstrap.h list.h
tau.o: tau.c tau.h list.h 
tree.o: tree.c tree.h list.h
vec.o: vec.c vec.h list.h
thresh_calc.o: thresh_calc.c bic.h tau.h tree.h list.h vec.h
student.o: student.cpp  list.h
draw.o: draw.c list.h tau.h tree.h

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
# Champions test
######################
champ_test: ../bin/champ_test


test/champ_test.o: test/champ_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -I . -I .. -o $@ $^ $(LDFLAGS)

../bin/champ_test: test/champ_test.o tree.o bic_setup.o bic_calculator.o tau.o champion_set.o draw.o read_file.o test/assert.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# Threshold test
######################
thresh_test: ../bin/thresh_test

test/thresh_test.o: test/thresh_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -I . -I .. -o $@ $^ $(LDFLAGS)

../bin/thresh_test: test/thresh_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o thresh_calc.o vec.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

######################
# Maxfreq test
######################
maxfreq_test: ../bin/maxfreq_test



test/maxfreq_test.o: test/maxfreq_test.c
	$(MAKE) -C $(@D) $(@F)
	# $(CC) $(CFLAGS) -I . -I .. -o $@ $^ $(LDFLAGS)

../bin/maxfreq_test: test/maxfreq_test.o tree.o bic_setup.o bic_calculator.o tau.o read_file.o test/assert.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
