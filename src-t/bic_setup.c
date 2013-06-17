/*
 * Methods for setup of the bic_calculator.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <float.h>

#include "bic.h"
#include "tree.h"
#include "list.h"

int max_word_size;
int sample_size;
double logN;
double scale = -1;

/*
 * We use 2 trees to calculate the BIC.
 * The prob_root tree is the one that holds the occurences, probabilities, degrees of freedom and Lw(X).
    In prefix order, necessary for conditional probabilities
 * The bic_root tree is the one on which the Vw(X) and Sw(X) (DELTAw(X))
    In suffix order, for computing tau
 * In the PROB tree, the probability of a node "0101" means p(1|010).
 * In the Prob tree nodes are stored in the conventional order.
 * From the roots, and its childs root->0->0->1 means that the string 001 occurred in the sample.
 * The Bic tree stores the nodes in reverse order: root->0->0->1 means that 100 occurred in the sample.
 * Both trees should not have nodes that didn't occured in the sample.
 */
Tree_node prob_root = NULL;
Tree_node bic_root = NULL;

/*
 * Definition of functions that are implemented below.
 */
void insert_sample(char* sample);
void calculate_probabilities(Tree_node node);
void set_degrees_freedom(Tree_node node);
void set_probability(Tree_node node);
void calculate_ell(Tree_node node);
Tree_node get_prob_node(Tree_node bic_node, Tree_node prob_node);
void set_mate(Tree_node bic_node, Tree_node prob_node);
void create_prob(char* sample, Tree_node root);
void create_bic(char* sample, Tree_node root);


/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char** samples, int depth, Tree_node prob_tree, Tree_node bic_tree) {
    
    max_word_size = depth;
    /* 
     * if (prob_root != NULL)	
     * 	free_node(prob_root);
     * prob_root = Tree_create(PROB); //new_Tree_node();
     */
    for (int i = 0; samples[i]; i++) 
	create_prob(samples[i], prob_tree);
    DEBS("create prob");
    
    sample_size = size_of_sample(prob_tree);
    logN = scale > 0 ? scale : log(sample_size);
    calculate_probabilities(prob_tree->child);
    DEBS("probab");
    
    // calculate_ell(prob_root->child);
    calculate_ell(prob_tree);
    //print_tree(prob_tree, "", depth+1); /* DEBUG */
    DEBS("ell");
    
    if(bic_tree != NULL) {
	/* 
         * if (bic_root != NULL) 
	 *     free_node(bic_root);
	 * bic_root = Tree_create(BIC);
         */
	for (int i = 0; samples[i]; i++)
	    create_bic(samples[i], bic_tree);
	DEBS("bic");
	
	set_mate(bic_tree, prob_tree);
	DEBS("mate");
    }
    //print_tree(bic_tree, "", depth+1);
}

/*
 * Insert one sample into the tree.
 */
/* 
 * void insert_sample(char* sample) {
 *   int sample_length = strlen(sample);
 * 
 *   // first iteration to create the probability tree
 *   for (int i = 0; sample[i] != '\0'; i++) {
 *     Tree_node current_node = prob_root;
 *     prob_root->p.occurrences++;
 *     
 *     // besides iterating on each letter, we use this second for to iterate on words
 *     // the stop condition uses max_word_size + 1 so we create an extra height, which is used for calculations
 *     for (int d = 0; d < max_word_size + 1 && d + i < sample_length; d++) {
 *       current_node = get_create_child_node(current_node, sample[i+d], PROB);
 *       current_node->p.occurrences++;
 *     }
 *   }
 * 
 *   // second iteration, to create the basic bic tree
 *   for (int i = 0; sample[i] != '\0'; i++) {
 *     Tree_node current_node = bic_root;
 *     // besides iterating on each letter, we use this second for to iterate on words
 *     for (int d = 0; d < max_word_size && i - d >= 0; d++) {
 *       current_node = get_create_child_node(current_node, sample[i-d], BIC);
 *     }
 *   }
 * }
 */

void create_prob(char* text, Tree_node root) {
    for (int i = 0; text[i] != '\0'; i++) {
	Tree_node node = root;
	root->p.occurrences++;
	// besides iterating on each letter, we use this second for to iterate on words
	// the stop condition uses max_word_size + 1 so we create an extra height, which is used for calculations
	for (int d = 0; d < max_word_size + 1 && text[ i + d ]; d++) {
	    node = get_create_child_node(node, text[i+d], PROB);
	    node->p.occurrences++;
	}
    }
}

void create_bic(char* text, Tree_node root) {
    // second iteration, to create the basic bic tree
    for (int i = 0; text[i] != '\0'; i++) {
	Tree_node node = root;
	// besides iterating on each letter, we use this second for to iterate on words
	for (int d = 0; d < max_word_size && i - d >= 0; d++) 
	    node = get_create_child_node(node, text[i-d], BIC);
    }
}
    
/*
 * Calculates the size of the samples, as the total number of occurrences of length 1 words
 */
int size_of_sample(Tree_node root) {

  int n = 0;

  //for  (Tree_node* node = prob_root->child; node != NULL; node = node->sibling) 
  ITERA(Tree_node, node, root->child, sibling)
      n += node->p.occurrences;

  return n;
}


/*
 * Calculates the probabilities for the given node, its childs and siblings
 */
void calculate_probabilities(Tree_node node) {
  // defense against null node, prob-data
  if (node == NULL) {
    return;
  }

  // calculates data for current node
  set_degrees_freedom(node);
  set_probability(node);

  // calculates for the other nodes
  calculate_probabilities(node->child);
  calculate_probabilities(node->sibling);
}

/*
 * Calculates the degrees of freedom for the given node.
 * The node should be from a PROB tree.
 */
void set_degrees_freedom(Tree_node node) {
  // defense against NULL or BIC nodes
  if (node == NULL) {
    return;
  }

  /* 
   * // df is the number of children a node has (that appeared in the sample), not the same as degrees of freedom
   * int df = 0;
   * // for (Tree_node* node = node->child; node != NULL; node = node->sibling)
   * ITERA(Tree_node*, node, node->child, sibling)
   *     df++;
   */

  node->p.df = degree(node);
}

/*
 * Calculates the probability for the current node, given the father.
 * If current node corresponds to "100", the corresponding probability is p(0|10)
 */
void set_probability(Tree_node current_node) {
  double occ = 0;
  ITERA(Tree_node, node, current_node->parent->child, sibling)
    occ += node->p.occurrences;
  current_node->p.probability = current_node->p.occurrences / occ;
}



/* will very slightly underestimate if parent is a suffix of the sample and is infrequent 
void set_probability(Tree_node* node) {
    node->p->probability = node->p->occurrences / (double) node->parent->p->occurrences;
}
*/


/*
 * Method that calculates ell for the given node, its siblings and children.
 */
void calculate_ell(Tree_node node) {
  // defense against null node, prob-data
  if (node == NULL) {
    return;
  }

  double value = 0;

  // for (Tree_node* child = node->child; child != NULL; child = child->sibling)
  ITERA(Tree_node, current_node, node->child, sibling)
      value += current_node->p.occurrences*log(current_node->p.probability);

  node->p.ell = value;
  if(!value && node->p.df > 1){ /* DEBUG */
      DEB("%s","Alarm: ");
      print_node(node,"",0);
  }
  
	  

  calculate_ell(node->child);
  calculate_ell(node->sibling);
}


/*
 * Returns the prob_node which represents the same word as the given bic_node
 */
Tree_node get_prob_node(Tree_node bic_node, Tree_node prob_node) {
  for (Tree_node node = bic_node; node->parent; node = node->parent)
      prob_node = get_child_node(prob_node, node->symbol);
  return prob_node;
}

/* 
 * Cache the results of get_prob_node, fewer walks down the tree
 */
void set_mate(Tree_node bic_node, Tree_node prob_node)
{
    if(!bic_node) return;
    
    bic_node->b.mate = get_prob_node(bic_node, prob_node);
    if(!bic_node->b.mate) {
	DEBS("ALARM:");
	//print_tree(prob_node, bic_node->depth);
	
	DEB("\nbic_node %p symb %c %d suf %s\n", bic_node, bic_node->symbol, bic_node->symbol, recover_suffix(bic_node));
	//exit(1);
	
    }
    
    set_mate(bic_node->child, prob_node);
    set_mate(bic_node->sibling, prob_node);
}


/* 
 * Parameters for the champion set calculation.
 * Both depend on the prob tree.
 * Max_c is a safe upper bound
 * eps is a guestimate, pending more theory
 */

double Max_c(Tree_node prob_node)
{
    int d = 0;
    ITERA(Tree_node, node, prob_node->child, sibling)
	d += degree(node);  // maybe should be - 1
    return -prob_node->p.ell / ( logN * (d - degree(prob_node)));    /* always correct */
}

double Eps(Tree_node prob_node)
{
    //return pow(degree(prob_node), -max_word_size) / (Max_c(prob_node)*1024);
    return 1. / (1 << 30);
    //return DBL_EPSILON;
}
