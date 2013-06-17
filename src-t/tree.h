/*
 * Definitions of the tree structure that will be used to hold the values.
 * The tree will be implemented over a binary tree where the left child is a
 * child of the current node and the right "child" is actually a sibling.
 */
#ifndef TREE_H_
#define TREE_H_



typedef struct tree_node *Tree_node;
typedef struct prob_data *Prob_data;
typedef struct bic_data *Bic_data;
typedef struct ell_data *L_data; // defined in bootstrap.h

typedef struct bivec * Bivec;
struct bivec
{
    double * small;
    double * large;
};


/*
 * Structure that holds the data that will be stored on each node of the probability tree
 */
struct prob_data {
    int occurrences; //on sample
    int df;
    double probability;
    double ell;
    double T;  //not used
    // Lw over all bootstrap samples
    Bivec L;
};

/*
 * structure that holds the data used to calculate a BIC tree.
 */
struct bic_data {
    double v;
    int critical;
    double lsum;
    int dfsum;
    Tree_node mate;
};


enum node_type {
    PROB,
    BIC
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
	struct prob_data p; // was prob_data
	struct bic_data b;  // was bic_data
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
 * If the child is created, it will also allocate space to one of the data structures (Prob_data or Bic_data) according to the given type.
 */
Tree_node get_create_child_node(Tree_node parent, char symbol, int type);

/*
 * Return the child of the given node that is related to the symbol from the alphabet.
 * Returns NULL if such a child does not exits.
 */
Tree_node get_child_node(Tree_node parent, char symbol);

/*
 * Frees the memory used by a node, its childs and siblings.
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
Tree_node node_of_word(Tree_node tree, char *w); // w must be 0 terminated
Tree_node node_of_suffix(Tree_node tree, char *w); // w must be 0 terminated

/* 
 * Get the word label of a node
 */
char* word_of_node(Tree_node node);


Bivec new_bivec(int small, int large);

void free_bivec(Bivec v);


#endif /* TREE_H_ */
