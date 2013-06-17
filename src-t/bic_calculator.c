/*
 * Methods for calculating the bic tree given a cost.
 */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tau.h"
#include "tree.h"
#include "list.h"

/*
 * these variables are defined in bic_setup.c
 */
extern int max_word_size;
extern Tree_node prob_root;
extern Tree_node bic_root;
extern int sample_size;
extern double logN;


/*
 * Definition of functions that are implemented below.
 */

void add_selected_words_to_tau(Tree_node node, Tau tau);
char* recover_suffix(Tree_node bic_node);
double recover_prob(char* suffix, Tree_node node);
void get_v(Tree_node bic_node, double c);
inline double node_part(const Tree_node prob_node, const double c) __attribute__((always_inline));
double children_part(Tree_node node);
double Lw(char * w, Tree_node prob_tree);

/*
 * Calculates the BIC given the penalty c.
 */
Tau calculate_BIC(double c, Tree_node bic_tree) {
    get_v(bic_tree, c * logN); // optimization
    DEBS("get_v");
    
    Tau tau = new_Tau();
    tau->c = c;
    add_selected_words_to_tau(bic_tree, tau);
    DEBS("Added");
    
    return tau;
}

/*
 * Calculates v values for a node, siblings and children, and decide if critical.
 */
void get_v(Tree_node bic_node, double clogN) {
    if (!bic_node) return;
    //Bic_data bnbd = bic_node->b;
    //if (!bnbd) return;
    
    Tree_node prob_node = bic_node->b.mate;
    DEB("prob_node %p\n", prob_node);
    
    // if node has no child, we use the node_part.
    if (bic_node->depth == max_word_size || bic_node->child == NULL) {
	bic_node->b.critical = 1;
	bic_node->b.v = node_part(prob_node, clogN);
	get_v(bic_node->sibling, clogN); // calculate siblings before returning
	return;
    }

    
    // calculate children, because it is needed for children_part calculation.
    get_v(bic_node->child, clogN);

    double n_p = node_part(prob_node, clogN);
    double c_p = children_part(bic_node);

    // critical is the opposite of delta
    if (c_p > n_p) {
	bic_node->b.critical = 0;
	bic_node->b.v = c_p;
    } else {
	bic_node->b.critical = 1;
	bic_node->b.v = n_p;
    }
    
    // calculate siblings before returning
    get_v(bic_node->sibling, clogN);
}

/*
 * Calculates log( n^{-c df(w)}Lw(X)) for a given node.
 * Either this or the value returned by children_part will become the v value.
 */
inline double node_part(const Tree_node prob_node, const double clogN) {
    return prob_node->p.ell - prob_node->p.df * clogN;
}

/*
 * Calculates \sum_{a \in A} V_{aw}(X) for a given node.
 * Either this or the value returned by node_part will become the v value.
 */
double children_part(Tree_node node) {

  double part = 0;
  
  ITERA(Tree_node, current_child, node->child, sibling)
     part += current_child->b.v;
  return part;
}





/*
 * Checks if the current node represents a word that must go to the final result.
 * Also checks children and siblings recursevely.
 */
void add_selected_words_to_tau(Tree_node node, Tau tau) {
  if(node == NULL) {
    return;
  }
  if (node->b.critical) {
    // this node is selected, must add it to the tau, descendants are not required
    char* suffix = recover_suffix(node);
    double prob = node->b.mate->p.probability;
    
    insert_tau_item(tau, suffix, prob);
    // tau->L += node->b->v;
    tau->L += node->b.mate->p.ell; // L has no penalties
  } else {
    //this node is not selected, but it's children may
    add_selected_words_to_tau(node->child, tau);
  }

  // but we must always check siblings
  add_selected_words_to_tau(node->sibling, tau);
}

/*
 * Recovers the suffix for the given node
 */
char* recover_suffix(Tree_node bic_node) {
  int length = bic_node->depth;
  MEM(char*, suffix, (char*) malloc((length+1)* sizeof(char))); // +1 for the \0 terminator
  
  Tree_node current_node = bic_node;
  for (int i = 0; i < length; i++) {
    suffix[i] = current_node->symbol;
    current_node = current_node->parent;
  }
  suffix[length] = '\0';
  return suffix;
}


/*
 * Recovers the probability for the given suffix.
 */
double recover_prob(char* suffix, Tree_node prob_node) {
    return node_of_word(prob_node, suffix)->p.probability;
}

/*
 * Returns the child of the given node which occurred the most.
 * If the depth of the node is max_word_size, then return the node.
 */
Tree_node most_frequent_child(Tree_node node) {
  if (node_depth(node) == max_word_size) {
    return node;
  }

  int max = 0;
  Tree_node max_node;
  Tree_node current_node = node->child;

  while (current_node != NULL) {
    Tree_node max_child = most_frequent_child(current_node);
    if (max_child->p.occurrences > max) {
      max = max_child->p.occurrences;
      max_node = max_child;
    }
    current_node = current_node->sibling;
  }
  return max_node;
}


	
Tree_node most_frequent_leaf(Tree_node node)
{
    Tree_node max_node;
    int max = 0;
    
    for(Tree_node t = next_node_depth(node, max_word_size); t;
	t = next_node_depth(t, max_word_size))
	if ( t->p.occurrences > max ) {
	    max_node = t;
	    max = t->p.occurrences;
	}
    return max_node;
}

    
/*
 * Return the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word(Tree_node root) {
    return word_of_node(most_frequent_leaf(root));
}

/* 
 * Lw is used for Ltau; what should be Lw if w is not in the tree?
 */
double Lw(char * w, Tree_node prob_tree)
{
    Tree_node node;
    // Compute Lw using the longest suffix of w in the tree
    while(!(node = node_of_word(prob_tree, w))) w++;
    return node->p.ell;
    /* 
     * // Alternative: w not in the sample, Lw = 0 (should be -\infty?
     * return ((prob_tree = node_of_word(prob_tree, w)))
     * 	    ? prob_tree->p.ell
     * 	    : 0;
     */
}



/*
 * Calculates Ltau on the current Prob Tree
 */
double L_tau(Tau tau) {
  double value = 0;
  ITERA(Tau_item, item, tau->item, next) {
    // go down the tree to find the correct node (node_of word)
    Tree_node node = prob_root;
    for (char* symbol = item->string; *symbol; symbol++)
      node = get_child_node(node, *symbol);

    if (node != NULL)
      value += node->p.ell;
  }
  return value;
}
