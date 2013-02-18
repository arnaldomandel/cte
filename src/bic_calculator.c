/*
 * Methods for calculating the bic tree given a cost.
 */
#include <stddef.h>

/*
 * these variables are defined in bic_setup.c
 */
extern int max_word_size;
extern Tree_node* prob_root;
extern Tree_node* bic_root;

/*
 * Definition of functions that are implemented below.
 */
int node_depth(Tree_node* node);
int size_of_sample();
void VwSw(Tree_node* node);
double Vw1(Tree_node* node);
double Vw2(Tree_node* node);

/*
 * Calculates the BIC given the penalty c.
 */
Tao* calculate_BIC(double c) {
  int n = size_of_sample();


  return 0;
}

/*
 * Calculates the Vw and Sw values for a node, siblings and childs.
 */
void VwSw(Tree_node* node) {
  if (node == NULL || node->bic_data == NULL) {
    return;
  }
  if (node_depth(node) == max_word_size || node->child == NULL) {
    node->bic_data->Sw = 0;
    node->bic_data->Vw = Vw1(node);
    VwSw(node->sibling);
    return;
  }
  VwSw(node->child);
  double vw1 = Vw1(node);
  double vw2 = Vw2(node);

  if (vw1 > vw2) {
    node->bic_data->Sw = 0;
    node->bic_data->Vw = vw1;
  } else {
    node->bic_data->Sw = 1;
    node->bic_data->Vw = vw2;
  }
  VwSw(node->sibling);
}

/*
 * Calculates n^{-cdf(w)}Lw(X) for a given node.
 * Either this or the value returned by Vw2 will become the Vw value.
 */
double Vw1(Tree_node* node) {
  return 0.0;
}

/*
 * Calculates \prod_{a \in A} V_{aw}(X) for a given node.
 * Either this or the value returned by Vw1 will become the Vw value.
 */
double Vw2(Tree_node* node) {
  return 0.0;
}


/*
 * Calculates the node depth.
 * If the given node is NULL, will return -1.
 * Return 0 for the root node.
 */
int node_depth(Tree_node* node) {
  int depth = -1;
  Tree_node current_node = node;
  while (current_node != NULL) {
    depth++;
    current_node = current_node->parent;
  }
  return depth;
}

/*
 * Calculates the size of the samples, as the sum of Xn(a) for every a in the alphabet.
 */
int size_of_sample() {
  int n = 0;
  Tree_node* current_node = prob_root->child;
  while (current_node != NULL) {
    n += current_node->prob_data->occurrences;
    current_node = current_node->sibling;
  }
  return n;
}
