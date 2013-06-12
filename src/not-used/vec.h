/* 
 * Vectors of doubles, arbitrary size
 */
/* Time-stamp: <2013/05/04 22:20:34 hutzpa [hutzpa] am> */

typedef struct vec *Vec;

struct vec {
    double *x;
    int len;
    int alloc;
};
    
Vec create_Vec(int space);

void sort_Vec(Vec v);

/* 
 * keeps a single copy of each value, v must be sorted
 */ 
void uniquefy_Vec(Vec v);

void print_Vec(Vec v);
