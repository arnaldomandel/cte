CFLAGS= -Wall -pedantic

all: cte

cte: main.o messages.o read_file.o
	cc -Wall -pedantic -o $@ $^

main.o: main.c messages.h read_file.h

messages.o: messages.h

read_file.o: read_file.h messages.h

clean:
	rm -f *.o
	rm -f cte
