/*
 * Tau is the structure given as the return tree.
 * It was called Tau not to be mixed up with the tree data structure.
 */
#ifndef TAO_H_
#define TAO_H_




typedef struct tau Tau;
typedef struct tau_item Tau_item;

struct tau {
  Tau_item* item;
  double c;
};

struct tau_item {
  char* string;
  double probability;
  Tau_item* next;
};

/*
 * Adds a item to the given tau.
 */
void insert_tau_item(Tau* tau, char* string, double prob);

/*
 * Returns a new Tau.
 */
Tau* new_Tau();


/*
 * Frees the memory allocated by this Tau.
 */
void free_Tau(Tau* t);


/*
 * Verifies if both taus are equals, ie, have the same words.
 */
int equals_Tau(Tau* t1, Tau* t2);


/*
 * Prints the Tau to the stdout.
 */
void print_Tau(Tau* tau);

#endif /* TAO_H_ */
