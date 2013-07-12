/*
 * Interface for the module that calculates the BIC trees.
 */
#ifndef BIC_H_
#define BIC_H_


#include "tree.h"
#include "tau.h"
/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char** samples, int depth);

/*
 * Calculates the champion Tree given the cost.
 */

Tau calculate_BIC(double c);

/*
 * Returns the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word();

/*
 * Returns the size of the sample used to calculate the BIC.
 */
int size_of_sample();



/*
 * Calculates Ltau on the current Prob Tree
 */
double L_tau(Tau t);


/*
 * Returns the prob_node which represents the same word as the given bic_node
 */
Tree_node get_prob_node(Tree_node bic_node);


/* 
 * Parameters for the champion set calculation.
 * Both depend on the prob tree.
 * Max_c is a safe upper bound
 * eps is a guestimate, pending more theory
 */

double Max_c();

double Eps();


#endif /* BIC_H_ */
