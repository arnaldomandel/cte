#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int d, c;

    if(argc !=2) {
	printf("Needs a single argument.\n");
	exit(1);
    }
    d = atoi(argv[1]);
    if(d < 1 || d > 9) {
	printf("Arg out of range.\n");
	exit(1);
    }
    while((c = getchar()) != EOF)
	putchar('0' + c % d);
}
