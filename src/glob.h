/* Time-stamp: <2013/05/28 11:39:43 benavuya.ime.usp.br [benavuya] am> */

#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DEBUG 0

#define ITERA(type,var,lista,prox) for(type var=lista; var; var=var->prox)

#define DEB(format, ...)  if(DEBUG)fprintf(stderr, "DEBUG: "format,__VA_ARGS__)
#define DEBS(msg)  if(DEBUG)fprintf(stderr, "DEBUG: %s\n", msg)
#define mess(format, ...) fprintf(stderr, format,__VA_ARGS__)
#define mesg(msg) fprintf(stderr, "%s\n", msg)

//#define MEM(x) if(!(x)) err(1,"%s line %d", __FILE__, __LINE__)

#define MEM(type, var, alloc) type var = alloc;	\
    if(!var) err(1,"%s line %d, allocating " #var, __FILE__, __LINE__)

#define MEMPZ(ptype, var, size)  MEM(ptype, var, (type) calloc(size, sizeof(ptype *)))

#define MEMTZ(type, var, size)  MEM(type *, var, (type *) calloc(size, sizeof(type)))

#define OPEN(file, fname, type)  if(!(file = fopen(fname, type))) err(1, "%s", fname)

#define TIMESTAMP  (((unsigned int)time(NULL) - 1367682157) >> 6)
/************************************************************* 
 *                 Global variables and types                *
 *************************************************************/

typedef char Filename[PATH_MAX];

extern char jobname[];

extern char *alphabet;

extern char issymbol[];
  
extern char real_filename[];
extern unsigned int depth;
extern unsigned int number_resamples;
extern unsigned int num_sizes;
extern char *renewalstr;
extern unsigned int seed;
extern double confid;
