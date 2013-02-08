/*
 * Implementation of the BIC calculator.
 */
#include <stddef.h>

#include "bic.h"

int max_word_size;

/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char* alphabet, char** samples, int depth) {
  max_word_size = depth;
  for (int i = 0; samples[i] != NULL; i++) {
    insert_sample(samples[i]);
  }
}

/*
 * Calculates the BIC given the penalty c.
 */
Tao calculate_BIC(double c) {
  return 0;
}

/*
 * Insert one sample into the tree.
 */
void insert_sample(char* sample) {
  for (int i = 0; sample[i] != '\0'; i++) {
    Tree_node* current_node = get_tree_root;
    // besides iterating on each letter, we use this second for to iterate on words
    for (int d = 0; d < max_word_size && i - d >= 0; d++) {
      current_node = get_node_child(current_node, sample[i-d]);
      current_node->data.occurrences++;
    }
  }
}
