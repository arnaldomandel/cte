/*
 * Methods for setup of the bic_calculator.
 */
/* Time-stamp: <2013/05/11 09:26:24 hutzpa [hutzpa] am> */
#include "glob.h"
#include "bic.h"
#include "tree.h"

#include <math.h>
//#include <float.h>

int max_word_size;
int sample_size;
double logN;
double scale = -1;

/*
 * We use 2 trees to calculate the BIC.
 * The pre tree is the one that holds the occurrences, probabilities, degrees of freedom and Lw(X).
 * The bic_root tree is the one on which the Vw(X) and Sw(X) (DELTAw(X))
 * In the pre tree, the probability of a node "0101" means p(1|010).
 * The suf tree stores the nodes in reverse order: root->0->0->1 means that 100 occurred in the sample.
 * Both trees should not have nodes that didn't occur in the sample.
 */

/*
 * Definition of functions that are implemented below.
 */
void insert_sample(char* sample);
void calculate_probabilities(Tree_node pre_node);
void set_degrees_freedom(Tree_node pre_node);
void set_probability(Tree_node pre_node);
void calculate_ell(Tree_node pre_node);
void set_mate(Tree_node suf_node, Tree_node pre_node);
void create_pre(Tree_node pre_root, char* sample);
void create_suf(Tree_node pre_root, char* sample);


/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the penalty c
 */
void setup_BIC(char** samples, int depth, Tree_node pre_tree, Tree_node suf_tree) {

    max_word_size = depth;
    /* 
     * if (pre_root != NULL)	
     * 	free_node(pre_root);
     * pre_root = Tree_create(PRE); //new_Tree_node();
     */
    for (int i = 0; samples[i]; i++) 
	create_pre(pre_tree, samples[i]);
    DEBS("create pre");
    
    sample_size = size_of_sample(pre_tree);
    calculate_probabilities(pre_tree->child);
    DEBS("probab");
    logN = scale > 0 ? scale : log(sample_size);
    // print_tree(pre_tree, depth+2); /* DEBUG */
    
    // calculate_ell(pre_root->child);
    calculate_ell(pre_tree);
    //print_tree(pre_tree, "", depth+1); /* DEBUG */
    DEBS("ell");
    
    if(suf_tree != NULL) {
	/* 
         * if (suf_root != NULL) 
	 *     free_node(suf_root);
	 * suf_root = Tree_create(SUF);
         */
	for (int i = 0; samples[i]; i++)
	    create_suf(suf_tree, samples[i]);
	DEBS("suf");
	
	set_mate(suf_tree, pre_tree);
	DEBS("mate");
    }
    // mesg("--------------------------------------------");  /* DEBUG */
    
    // print_tree(suf_tree, depth+1); /* DEBUG */
}


void create_pre(Tree_node pre_root, char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
	Tree_node node = pre_root;
	pre_root->p.occurrences++;
	// besides iterating on each letter, we use this second for to iterate on words
	// the stop condition uses max_word_size + 1 so we create an extra level, which is used for calculations
	for (int d = 0; d < max_word_size + 1 && text[ i + d ]; d++) {
	    node = get_create_child_node(node, text[i+d], PRE);
	    node->p.occurrences++;
	}
    }
}

void create_suf(Tree_node suf_root, char* text) {
    // second iteration, to create the basic suf tree
    for (int i = 0; text[i] != '\0'; i++) {
	Tree_node node = suf_root;
	// besides iterating on each letter, we use this second for to iterate on words
	for (int d = 0; d < max_word_size && i - d >= 0; d++) 
	    node = get_create_child_node(node, text[i-d], SUF);
    }
}
    
/*
 * Calculates the size of the samples, as the total number of occurrences of length 1 words
 */
int size_of_sample(Tree_node root) {

  int n = 0;

  //for  (Tree_node* node = pre_root->child; node != NULL; node = node->sibling) 
  ITERA(Tree_node, node, root->child, sibling)
      n += node->p.occurrences;

  return n;
}


/* 
 * Upper bound on the number of nodes, 
 * given the alphabet, sample size (extracted from the tree) and depth
 */
int max_possible_nodes(Tree_node pre_tree, int depth)
{
    int alf = degree(pre_tree);
    int level_i = 1, i;
    int size = size_of_sample(pre_tree);
    // mess("num: %lld\n",num);
  
    for (i = 0; i <= depth && level_i <= size - i + 1; i++)
	level_i *= alf;
    // if (level_i <= size - i + 1) level_i *= alf;
    
    return (level_i-1)/(alf-1)+(depth-i+2)*size;
}


/*
 * Calculates the probabilities and degrees of freedom for the given node, its children and siblings
 */
void calculate_probabilities(Tree_node pre_node) {
  // defense against null node, pre-data
  if (pre_node == NULL) {
    return;
  }

  // calculates data for current node
  set_degrees_freedom(pre_node);
  set_probability(pre_node);

  // calculates for the other nodes
  calculate_probabilities(pre_node->child);
  calculate_probabilities(pre_node->sibling);
}

/*
 * Calculates the degrees of freedom for the given node.
 */
void set_degrees_freedom(Tree_node pre_node) {
  // defense against NULL or SUF nodes
  if (pre_node == NULL) {
    return;
  }

  /* 
   * // df is the number of children a node has (that appeared in the sample), not the same as degrees of freedom
   * int df = 0;
   * // for (Tree_node* node = node->child; node != NULL; node = node->sibling)
   * ITERA(Tree_node*, node, node->child, sibling)
   *     df++;
   */

  pre_node->p.df = degree(pre_node);
}

/*
 * Calculates the probability for the current node, given the father.
 * If current node corresponds to "100", the corresponding probability is p(0|10)
 */
void set_probability(Tree_node pre_node) {
  double occ = 0;
  ITERA(Tree_node, node, pre_node->parent->child, sibling)
    occ += node->p.occurrences;
  pre_node->p.probability = pre_node->p.occurrences / occ;
}



/* will very slightly underestimate if parent is a suffix of the sample and is infrequent 
void set_probability(Tree_node* node) {
    node->p->probability = node->p->occurrences / (double) node->parent->p->occurrences;
}
*/


/*
 * Method that calculates ell for the given node, its siblings and children.
 */
void calculate_ell(Tree_node pre_node) {
  // defense against null node, pre-data
  if (pre_node == NULL) {
    return;
  }

  double value = 0;

  // for (Tree_node* child = node->child; child != NULL; child = child->sibling)
  ITERA(Tree_node, node, pre_node->child, sibling)
      value += node->p.occurrences*log(node->p.probability);

  pre_node->p.ell = value;
  if(!value && pre_node->p.df > 1){ /* DEBUG */
      DEB("%s","Alarm: ");
      print_node(pre_node,"",0);
  }
  calculate_ell(pre_node->child);
  calculate_ell(pre_node->sibling);
}


/*
 * Returns the pre_node which represents the same word as the given suf_node
 */
Tree_node get_pre_node(Tree_node suf_node, Tree_node pre_node) {
  for (Tree_node node = suf_node; node->parent; node = node->parent)
      pre_node = get_child_node(pre_node, node->symbol);
  return pre_node;
}

/* 
 * Cache the results of get_pre_node, fewer walks down the tree
 */
void set_mate(Tree_node suf_node, Tree_node pre_node)
{
    if(!suf_node) return;
    
    suf_node->s.mate = get_pre_node(suf_node, pre_node);
    if(!suf_node->s.mate) {
	DEBS("ALARM:");
	//print_tree(pre_node, suf_node->depth);
	DEB("\nsuf_node %p symb %c %d suf %s\n", suf_node, suf_node->symbol, suf_node->symbol, suffix_of_node(suf_node));
	//exit(1);
    }
    set_mate(suf_node->child, pre_node);
    set_mate(suf_node->sibling, pre_node);
}


/* 
 * Parameters for the champion set calculation.
 * Both depend on the pre tree.
 * Max_c is a safe upper bound
 * eps is a guestimate, pending more theory
 */

double Max_c(Tree_node pre_node)
{
    int d = 0;
    ITERA(Tree_node, node, pre_node->child, sibling)
	d += degree(node);  // maybe should be - 1
    return -pre_node->p.ell / ( logN * (d - degree(pre_node)));    /* always correct */
}

double Eps(Tree_node pre_node)
{
    //return pow(degree(pre_node), -max_word_size) / (Max_c(pre_node)*1024);
    return 1. / (1 << 30);
    //return DBL_EPSILON;
}

