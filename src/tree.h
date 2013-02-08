/*
 * Definitions of the tree structure that will be used to hold the values.
 * The tree will be implemented over a binary tree where the left child is a
 * child of the current node and the right "child" is actually a sibling.
 */

typedef struct tree_node Tree_node;

typedef struct node_data Node_data;

/*
 * Structure that hold the data that will be stored on each node
 */
struct node_data {
  int occurrences; //on sample
  double probability;
  int degreesFreedom;
  double Lw;
  char symbol;
};

/*
 * The tree node structure.
 */
struct tree_node {
  Node_data data;
  Tree_node* child;
  Tree_node* sibling;
  Tree_node* parent;
};


/*
 * Return the child of the given node that is related to the symbol from the alphabet.
 * If such a child does not exits, then this method creates one and returns it.
 */
Tree_node* get_node_child(Tree_node* parent, char symbol);

/*
 * Returns the root of the tree.
 */
Tree_node* get_tree_root();
