#include <err.h>
#include <stdio.h>

#define DEBUG 0

#define ITERA(type,var,lista,prox) for(type var=lista; var; var=var->prox)

#define DEB(format, ...)  if(DEBUG)printf("DEBUG: "format,__VA_ARGS__)
#define DEBS(mess)  if(DEBUG)printf("DEBUG: %s\n", mess)

//#define MEM(x) if(!(x)) err(1,"%s line %d", __FILE__, __LINE__)

#define MEM(type, var, alloc) type var = alloc;	\
    if(!var) err(1,"%s line %d, allocating " #var, __FILE__, __LINE__)

#define MEMZ(type, var, size)  MEM(type, var, (type) calloc(size, sizeof(type *)))
