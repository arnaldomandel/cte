#include "glob.h"
#include "params.h"
#include <math.h>
#include <time.h>

void usage()
{
}

char randchar(int k) 
{
    return 'a'+ rand() % k;
}

int main(int argc, char **argv)
{
    Filename filename;
    FILE *file;
    int alf = 0, size = 0, seed = 0;
    int depth = 5, opt;
    double chances = 0.8;
    int i, top, last;
    char buf[10];
    
    while((opt = getopt(argc, argv, "a:s:d:c:S:")) != -1) 
	switch ( opt ) {
	    // SOPT('f', "filename", filename);
	    IOPT('a', "alphabet size", alf);
	    IOPT('s', "size", size);
	    IOPT('d', "depth", depth);
	    FOPT('c', "chances", chances);
	    IOPT('S', "seed", seed);
	}
    if(erropt || !alf || !size) {
	usage();
	exit(1);
    }
    // starting the random number generator
    if(!seed) seed = (unsigned int)time(NULL);
    srand( seed );
    mess("Seed: %d.\n", seed);

    sprintf(filename, "skew-a%d-d%d-c%f-S%d-%06d", alf, depth, chances, seed,
	    TIMESTAMP);
    OPEN(file, filename, "w");

    MEMTZ(char, out, 1 << 20);
    
    // init
    // for(last = out; last < out + depth; *last++ = randchar());
    for(last = 0; last < depth; out[last++] = randchar(alf));
    
    while(last < size){
	double cut = chances;
	for(i = last - 1; i > last - depth && out[i] > out[i-1]; i--)
	    cut *= chances;
	double draw = rand() * 1.0 / RAND_MAX;
	if(draw < cut)
	    out[last] = 'a' +  trunc(draw * (out[last-1] - 'a') / cut);
	else
	    out[last] = out[last-1] +
		trunc((draw - cut) * ('a' + alf - out[last-1]) / (1 - cut));
	last++;
    }
    fputs(out, file);
    fclose(file);
    mesg("generated");
    
    // generate the real tree
    strcat(filename, ".real");
    OPEN(file, filename, "w");
    buf[depth] = '\0';
    buf[top = depth - 1] = 'a'-1;
    while( top < depth ) {
	buf[top]++;
	if(buf[top] == 'a')
	    fprintf(file, " %s", buf+top);
	else if(buf[top] >= 'a' + alf)
	    top++;
	else if(top == depth - 1 || (top && buf[top] < buf[top+1]) )
	    buf[--top] = 'a' - 1;
	else
	    fprintf(file, " %s", buf+top);
    }
    fclose(file);
    return 0;
}

