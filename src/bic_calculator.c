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
void reset_bic(Tree_node* node);

/*
 * Calculates the BIC given the penalty c.
 */
Tao* calculate_BIC(double c) {
  reset_bic(bic_root->child);
  return 0;
}


/*
 * Resets the values on the bic tree so they are all calculated again
 */
void reset_bic(Tree_node* node) {
  // defense against null nodes and non bic nodes
  if (node == NULL || node->bic_data == NULL) {
    return;
  }
  node->bic_data->calculated = 0;

  reset_bic(node->child);
  reset_bic(node->sibling);
}
