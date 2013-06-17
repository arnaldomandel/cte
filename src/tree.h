/*
 * Definitions of the tree structure that will be used to hold the values.
 * The tree will be implemented over a binary tree where the left child is a
 * child of the current node and the right "child" is actually a sibling.
 */
/* Time-stamp: <2013/05/26 23:37:29 hutzpa [hutzpa] am> */
#ifndef TREE_H_
#define TREE_H_

typedef struct tree_node *Tree_node;
typedef struct pre_data *Pre_data;
typedef struct suf_data *Suf_data;
typedef struct ell_data *L_data; // defined in bootstrap.h

// matrix; one row for sample size, one column for sample number
typedef double **siz_by_samp;  

/*
 * Structure that holds the data that will be stored on each node of the probability tree
 */
struct pre_data {
    int occurrences; //on sample
    int df;
    double probability;
    double ell;
    // Lw over all bootstrap samples
    siz_by_samp L;
};

/*
 * structure that holds the data used to calculate a BIC tree.
 */
struct suf_data {
    double v;
    int critical;
    double lsum;
    int dfsum;
    Tree_node mate;
};


enum node_type {
    PRE,
    SUF
};

/*
 * The tree node structure.
 */
struct tree_node {
    unsigned char type;
    unsigned char symbol;
#if __GNUC_MINOR__ > 5
    union 
    {
#endif
	struct pre_data p; // was prob_data
	struct suf_data s;  // was bic_data
#if __GNUC_MINOR__ > 5
    };
#endif
    int depth;
    Tree_node child;
    Tree_node sibling;
    Tree_node parent;
};


/*
 * Return the child of the given node that is related to the symbol from the alphabet.
 * If such a child does not exits, then this method creates one and returns it.
 */
Tree_node get_create_child_node(Tree_node parent, char symbol, int type);

/*
 * Return the child of the given node that labelled by the symbol from the alphabet.
 * Returns NULL if such a child does not exist.
 */
Tree_node get_child_node(Tree_node parent, char symbol);

/*
 * Frees the memory used by a node, its children and siblings.
 */
void free_node(Tree_node node);


/*
 * Instantiates a new Tree_node and set its default values
 */
Tree_node new_Tree_node();

/* 
 * Creates an empty tree
 */ 
Tree_node Tree_create(int type);


/*
 * Sort of pprinting, for debugging purposes
 */
void print_node(Tree_node node, char* label, int depth);
void print_tree(Tree_node tree, int depth);

/* 
 * Prints tree and transition probs to file
 */
void fprint_tree_probs(FILE *file, Tree_node tree, int depth);

/*
 * The number of children 
 */
int degree(Tree_node node);


/* 
 * Number of nodes in the tree
 */
int tree_size(Tree_node root);

/*
 * Calculates the node depth.
 * If the given node is NULL, will return -1.
 * Return 0 for the root node.
 */
int node_depth(Tree_node node);

/* 
 * Iterator for nodes at a given depth.  Returns NULL at the end.
 */

Tree_node next_node_depth(Tree_node node, int depth);

/* 
 * Go down a digital tree
 */
Tree_node node_of_word(Tree_node tree, const char *w); // w must be 0 terminated
Tree_node node_of_suffix(Tree_node tree, const char *w); // w must be 0 terminated
//Tree_node node_of_max_suffix(Tree_node tree, char *w);

/* 
 * Get the label of a node
 */
char* word_of_node(Tree_node pre_node);
char* suffix_of_node(Tree_node suf_node);


#endif /* TREE_H_ */
