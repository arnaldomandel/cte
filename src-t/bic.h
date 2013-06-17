/*
 * Interface for the module that calculates the BIC trees.
 */
#ifndef BIC_H_
#define BIC_H_


#include "tree.h"
#include "tau.h"

#define BOTH 1
#define JUST_PROB 0

/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char** samples, int depth, Tree_node prob_tree, Tree_node bic_tree);

/*
 * Calculates the champion Tree given the cost.
 */

Tau calculate_BIC(double c, Tree_node bic_tree);

/*
 * Returns the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word(Tree_node root);

/*
 * Returns the size of the sample used to calculate the BIC.
 */
int size_of_sample(Tree_node root);

/* 
 * Adds critical words from the bic tree to tau
 */
void add_selected_words_to_tau(Tree_node bic_node, Tau tau);

/*
 * Calculates Ltau on the current Prob Tree
 */
double L_tau(Tau t);


/*
 * Returns the prob_node which represents the same word as the given bic_node
 */
Tree_node get_prob_node(Tree_node bic_node, Tree_node prob_node);


/* 
 * Parameters for the champion set calculation.
 * Both depend on the prob tree.
 * Max_c is a safe upper bound
 * eps is a guestimate, pending more theory
 */

double Max_c(Tree_node prob_node);

double Eps(Tree_node prob_node);

char* recover_suffix(Tree_node bic_node);
double Lw(char * w, Tree_node prob_tree);


#endif /* BIC_H_ */
