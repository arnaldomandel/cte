/*
 * Methods for calculating the bic tree given a cost.
 */
/* Time-stamp: <2013/05/10 18:55:23 benavuya.ime.usp.br [benavuya] am> */
#include "glob.h"
#include "tau.h"
#include "tree.h"

#include <math.h>

/*
 * these variables are defined in bic_setup.c
 */
extern int max_word_size;
extern int sample_size;
extern double logN;


/*
 * Definition of functions that are implemented below.
 */

void add_selected_words_to_tau(Tree_node node, Tau tau);
double recover_prob(Tree_node pre_node, char* suffix);
void get_v(Tree_node bic_node, double c);
inline double node_part(const Tree_node pre_node, const double c) __attribute__((always_inline));
double children_part(Tree_node node);
double Lw(Tree_node pre_tree, char * w);

/*
 * Calculates the BIC given the penalty c.
 */
Tau calculate_BIC(Tree_node suf_tree, double c) {
    get_v(suf_tree, c * logN); // optimization
    DEBS("get_v");
    
    Tau tau = new_Tau();
    tau->c = c;
    add_selected_words_to_tau(suf_tree, tau);
    DEBS("Added");
    
    return tau;
}

/*
 * Calculates v values for a node, siblings and children, and decide if critical.
 */
void get_v(Tree_node suf_node, double clogN) {
    if (!suf_node) return;
    //Bic_data bnbd = bic_node->b;
    //if (!bnbd) return;
    
    Tree_node pre_node = suf_node->s.mate;
    //mess("pre_node %p\n", pre_node); /* DEBUG */
    
    // if node has no child, we use the node_part.
    if (suf_node->depth == max_word_size || suf_node->child == NULL) {
	suf_node->s.critical = 1;
	suf_node->s.v = node_part(pre_node, clogN);
	get_v(suf_node->sibling, clogN); // calculate siblings before returning
	return;
    }

    
    // calculate children, because it is needed for children_part calculation.
    get_v(suf_node->child, clogN);

    double n_p = node_part(pre_node, clogN);
    double c_p = children_part(suf_node);

    // critical is the opposite of delta
    if (c_p > n_p) {
	suf_node->s.critical = 0;
	suf_node->s.v = c_p;
    } else {
	suf_node->s.critical = 1;
	suf_node->s.v = n_p;
    }
    
    // calculate siblings before returning
    get_v(suf_node->sibling, clogN);
}

/*
 * Calculates log( n^{-c df(w)}Lw(X)) for a given node.
 * Either this or the value returned by children_part will become the v value.
 */
inline double node_part(const Tree_node pre_node, const double clogN) {
    // print_node(pre_node, "", 5); /* DEBUG */
    
    return pre_node->p.ell - pre_node->p.df * clogN;
}

/*
 * Calculates \sum_{a \in A} V_{aw}(X) for a given node.
 * Either this or the value returned by node_part will become the v value.
 */
double children_part(Tree_node node) {

  double part = 0;
  
  ITERA(Tree_node, current_child, node->child, sibling)
     part += current_child->s.v;
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
  if (node->s.critical) {
    // this node is selected, must add it to the tau, descendants are not required
    char* suffix = suffix_of_node(node);
    double prob = node->s.mate->p.probability;
    
    insert_tau_item(tau, suffix, prob);
    // tau->L += node->b->v;
    tau->L += node->s.mate->p.ell; // L has no penalties
  } else {
    //this node is not selected, but it's children may
    add_selected_words_to_tau(node->child, tau);
  }

  // but we must always check siblings
  add_selected_words_to_tau(node->sibling, tau);
}



/*
 * Recovers the probability for the given suffix.
 */
double recover_prob(Tree_node pre_node, char* suffix) {
    return node_of_word(pre_node, suffix)->p.probability;
}

/*
 * Returns the child of the given node which occurred the most.
 * If the depth of the node is max_word_size, then return the node.
 */
Tree_node most_frequent_child(Tree_node pre_node) {
  if (node_depth(pre_node) == max_word_size) 
    return pre_node;

  int max = 0;
  Tree_node max_node;
  Tree_node current_node = pre_node->child;

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


	
Tree_node most_frequent_leaf(Tree_node pre_node)
{
    Tree_node max_node;
    int max = 0;
    
    for(Tree_node t = next_node_depth(pre_node, max_word_size); t;
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
char* most_frequent_word(Tree_node pre_root) {
    return word_of_node(most_frequent_leaf(pre_root));
}

/* 
 * Lw is used for Ltau; what should be Lw if w is not in the tree?
 */
double Lw(Tree_node pre_tree, char * w)
{
    Tree_node node;
    // Compute Lw using the longest suffix of w in the tree
    while(!(node = node_of_word(pre_tree, w))) w++;
    return node->p.ell;
}



/* 
 * /\*
 *  * Calculates Ltau on the current Pre Tree
 *  *\/
 * double L_tau(Tau tau) {
 *   double value = 0;
 *   ITERA(Tau_item, item, tau->item, next) {
 *     // go down the tree to find the correct node (node_of word)
 *     Tree_node node = pre_root;
 *     for (char* symbol = item->string; *symbol; symbol++)
 *       node = get_child_node(node, *symbol);
 * 
 *     if (node != NULL)
 *       value += node->p.ell;
 *   }
 *   return value;
 * }
 */
