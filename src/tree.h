typedef struct tree_node Node;

typedef struct node_data Node_data;

struct node_data {
  int occurrences; //on sample
  double probability;
  int degreesFreedom;
  double Lw;
  char symbol;
};

struct tree_node {
  Node_data data;
  Node* child;
  Node* sibling;
};


/*
 * Return the child of the given node that is related to the symbol from the alphabet.
 * If such a child does not exits, then this method creates one and returns it.
 */
Node* get_child(Node parent, char symbol);
