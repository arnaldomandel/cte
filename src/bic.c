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
 * In the PROB tree, the probability of a node "0101" means p(1|010).
 */
Tree_node* prob_root;
Tree_node* bic_root;

/*
 * Definition of functions that are implemented below.
 */
void insert_sample(char* sample);
void calculate_probabilities();
void set_degrees_freedom(Tree_node* node);
void set_probability(Tree_node* node);


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

  calculate_probabilities(prob_root->child);

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
    // the stop condition uses max_word_size + 1 so we create an extra height, which is used for degree of freedom calculation
    for (int d = 0; d < max_word_size + 1 && d + i < sample_length; d++) {
      current_node = get_create_node_child(current_node, sample[i+d], PROB);
      current_node->prob_data->occurrences++;
      if (d == max_word_size) {
        // here we indicate that this extra height node should not be used for anything but degree of freedom
        current_node->prob_data->placeholder = 1;
      }
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

/*
 * Calculates the probabilities for the given node, its childs and siblings
 */
void calculate_probabilities(Tree_node* node) {
  // defense against null node, prob-data or placeholders
  if (node == NULL || node->prob_data == NULL || node->prob_data->placeholder) {
    return;
  }
  set_degrees_freedom(node);
  set_probability(node);

  // calculates for the other nodes
  if (node->child != NULL) {
    calculate_probabilities(node->child);
  }
  if (node->sibling != NULL) {
    calculate_probabilities(node->sibling);
  }
}

/*
 * Calculates the degrees of freedom for the given node.
 * The node should be from a PROB tree.
 */
void set_degrees_freedom(Tree_node* node) {
  // defense against NULL or BIC nodes
  if (node == NULL || node->prob_data == NULL) {
    return;
  }

  // df is the number os childs a node has (that appeared in the sample), the same as degrees of freedom
  Tree_node* current_node = node->child;
  int df = 0;
  while (current_node != NULL) {
    if (current_node->prob_data->occurrences > 0) {
      df++;
    }
    current_node = current_node->sibling;
  }
  node->prob_data->degrees_freedom = df;
}

/*
 *
 */
void set_probability(Tree_node* node) {
  int sum_occurrences = 0;
  Tree_node* current_node = node->parent->child;
  while (current_node != NULL) {
    sum_occurrences += current_node->prob_data->occurrences;
    current_node = current_node->sibling;
  }
  node->prob_data->probability = node->prob_data->occurrences / (double) sum_occurrences;
}
