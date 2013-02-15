/*
 * Implementation of the BIC calculator.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bic.h"
#include "tree.h"

int max_word_size;

/*
 * We use 2 trees to calculate the BIC.
 * The prob_root tree is the one that holds the occurences, probabilities, degrees of freedom and Lw(X).
 * The bic_root tree is the one on which the Vw(X) and Sw(X) (DELTAw(X))
 */
Tree_node* prob_root;
Tree_node* bic_root;

/*
 * Definition of the function that insert one sample on the tree.
 */
void insert_sample(char* sample);


/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char* alphabet, char** samples, int depth) {
  bic_root = malloc(sizeof(Tree_node));
  prob_root = malloc(sizeof(Tree_node));
  max_word_size = depth;
  for (int i = 0; samples[i] != NULL; i++) {
    insert_sample(samples[i]);
  }

}

/*
 * Calculates the BIC given the penalty c.
 */
Tao* calculate_BIC(double c) {
  return 0;
}

/*
 * Insert one sample into the tree.
 * There are 2 fors in this function.
 * The first is to iterate over every letter on the sample.
 * The second iterates back so that, inside it, we read words backwards.
 * These words that are at most _max_word_size_ in length.
 * Inside the second for, we also go deeper into the tree.
 */
void insert_sample(char* sample) {
  int sample_length = strlen(sample);
  // first iteration to create the probability tree
  for (int i = 0; sample[i] != '\0'; i++) {
    Tree_node* current_node = prob_root;
    // besides iterating on each letter, we use this second for to iterate on words
    for (int d = 0; d < max_word_size && d + i < sample_length; d++) {
      current_node = get_create_node_child(current_node, sample[i+d], PROB);
      current_node->prob_data->occurrences++;
    }
  }

  // second iteration, to create the basic bic tree
  for (int i = 0; sample[i] != '\0'; i++) {
    Tree_node* current_node = bic_root;
    // besides iterating on each letter, we use this second for to iterate on words
    for (int d = 0; d < max_word_size && i - d >= 0; d++) {
      current_node = get_create_node_child(current_node, sample[i-d], BIC);
    }
  }
}
