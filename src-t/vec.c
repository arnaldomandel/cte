#include "vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/* 
 * Vectors of doubles, arbitrary size
 */

Vec create_Vec(int space)
{
    Vec v = (Vec) malloc(sizeof (Vec[0]));
    v->x = (double *) calloc(space, sizeof (double));
    v->alloc = space;
    return  v;
}

static int
cmpdouble(const void *p1, const void *p2) // compares for reverse sorting
{
    double *a = (double *) p1, *b = (double *) p2;
    
    return (*a > *b) ? -1 : (*a < *b);
}

void sort_Vec(Vec v)
{
    qsort(v->x, v->len, sizeof (double), cmpdouble);
}


/* 
 * keeps a single copy of each value, v must be sorted
 */ 
void uniquefy_Vec(Vec v)
{
    int cur, t;
    for(cur = t = 0; t < v->len; t ++)
	if ( v->x[t] < v->x[cur] )
	    v->x[++cur] = v->x[t];
    v->len = cur + 1;
}



void print_Vec(Vec v)
{
    printf("len=%d (%d)", v->len, v->alloc);

    for (int i = 0; i < v->len; i++) {
	if ( i % 10 == 0 ) printf("\n");
	printf("%f ", v->x[i]);
    }
    printf("\n");
}
