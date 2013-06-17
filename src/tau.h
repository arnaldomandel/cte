/*
 * Tau is the structure given as the return tree.
 * It was called Tau not to be mixed up with the tree data structure.
 */
/* Time-stamp: <2013/05/26 17:33:52 hutzpa [hutzpa] am> */
#ifndef TAO_H_
#define TAO_H_

typedef struct tau *Tau;
typedef struct tau_item *Tau_item;

struct tau {
    Tau_item item, last;
    double c;
    double L;
};

struct tau_item {
  char* string;
  double probability;
  Tau_item next;
};

/*
 * Adds a item to the given tau.
 */
void insert_tau_item(Tau tau, char* string, double prob);

/*
 * Returns a new Tau.
 */
Tau new_Tau();


/*
 * Frees the memory allocated by this Tau.
 */
void free_Tau(Tau t);

/*
 * Verifies if both taus are equal, ie, have the same words.
 */
int equals_Tau(Tau t1, Tau t2);

/*
 * Prints Tau to a file.
 */
void fprint_Tau(FILE *f, Tau tau);
void fpprint_Tau(FILE *f, Tau tau, double L0);

#endif /* TAO_H_ */
