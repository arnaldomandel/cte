/*
 * Interface for the module that calculates the BIC trees.
 */
/* Time-stamp: <2013/05/11 08:43:09 hutzpa [hutzpa] am> */
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
void setup_BIC(char** samples, int depth, Tree_node pre_tree, Tree_node suf_tree);

/*
 * Calculates the champion Tree given the cost.
 */

Tau calculate_BIC(Tree_node suf_tree, double c);

/*
 * Returns the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word(Tree_node pre_root);

/*
 * Returns the size of the sample used to calculate the BIC.
 */
int size_of_sample(Tree_node root);

/* 
 * Upper bound on the number of nodes, 
 * given the alphabet, sample size (extracted from the tree) and depth
 */
int max_possible_nodes(Tree_node pre_tree, int depth);

/* 
 * Adds critical words from the suf tree to tau
 */
void add_selected_words_to_tau(Tree_node suf_node, Tau tau);

/*
 * Calculates Ltau on the current Pre Tree
 */
double L_tau(Tau t);


/*
 * Returns the pre_node which represents the same word as the given suf_node
 */
Tree_node get_pre_node(Tree_node suf_node, Tree_node pre_node);


/* 
 * Parameters for the champion set calculation.
 * Both depend on the pre tree.
 * Max_c is a safe upper bound
 * eps is a guestimate, pending more theory
 */

double Max_c(Tree_node pre_node);

double Eps(Tree_node pre_node);

double Lw(Tree_node pre_tree, char * w);


#endif /* BIC_H_ */
