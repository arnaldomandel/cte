/*
 * Definitions of the tree structure that will be used to hold the values.
 * The tree will be implemented over a binary tree where the left child is a
 * child of the current node and the right "child" is actually a sibling.
 */

typedef struct tree_node Tree_node;

typedef struct prob_data Prob_data;
typedef struct bic_data Bic_data;

/*
 * Structure that holds the data that will be stored on each node of the probability tree
 */
struct prob_data {
  int occurrences; //on sample
  double probability;
  int degrees_freedom;
  double Lw;
};

/*
 * structure that holds the data used to calculate a BIC tree.
 */
struct bic_data {
  double Vw;
  int Sw; // actually DELTAw
};

enum node_type {
  PROB,
  BIC
};

/*
 * The tree node structure.
 */
struct tree_node {
  Prob_data* prob_data;
  Bic_data* bic_data;
  char symbol;
  Tree_node* child;
  Tree_node* sibling;
  Tree_node* parent;
};


/*
 * Return the child of the given node that is related to the symbol from the alphabet.
 * If such a child does not exits, then this method creates one and returns it.
 * If the child is created, it will also allocate space to one of the data structures (Prob_data or Bic_data) according to the given type.
 */
Tree_node* get_create_node_child(Tree_node* parent, char symbol, int type);

/*
 * Frees the memory used by a node, its childs and siblings.
 */
void free_node(Tree_node* node);
