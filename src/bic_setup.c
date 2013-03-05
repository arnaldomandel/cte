/*
 * Methods for setup of the bic_calculator.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bic.h"
#include "tree.h"

int max_word_size;
int sample_size;

/*
 * We use 2 trees to calculate the BIC.
 * The prob_root tree is the one that holds the occurences, probabilities, degrees of freedom and Lw(X).
 * The bic_root tree is the one on which the Vw(X) and Sw(X) (DELTAw(X))
 * In the PROB tree, the probability of a node "0101" means p(1|010).
 * In the Prob tree nodes are stored in the conventional order.
 * From the roots, and its childs root->0->0->1 means that the string 001 occurred in the sample.
 * The Bic tree stores the nodes in reverse order: root->0->0->1 means that 100 occurred in the sample.
 * Both trees should not have nodes that didn't occured in the sample.
 */
Tree_node* prob_root;
Tree_node* bic_root;

/*
 * Definition of functions that are implemented below.
 */
void insert_sample(char* sample);
void calculate_probabilities(Tree_node* node);
void set_degrees_freedom(Tree_node* node);
void set_probability(Tree_node* node);
void calculate_Lw(Tree_node* node);

/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char** samples, int depth) {
  if (bic_root != NULL) {
    free_node(bic_root);
  }
  if (prob_root != NULL) {
    free_node(prob_root);
  }
  bic_root = new_Tree_node();
  prob_root = new_Tree_node();
  max_word_size = depth;

  for (int i = 0; samples[i] != NULL; i++) {
    insert_sample(samples[i]);
  }
  sample_size = size_of_sample();

  calculate_probabilities(prob_root->child);
  calculate_Lw(prob_root->child);
}

/*
 * Insert one sample into the tree.
 */
void insert_sample(char* sample) {
  int sample_length = strlen(sample);

  // first iteration to create the probability tree
  for (int i = 0; sample[i] != '\0'; i++) {
    Tree_node* current_node = prob_root;
    // besides iterating on each letter, we use this second for to iterate on words
    // the stop condition uses max_word_size + 1 so we create an extra height, which is used for calculations
    for (int d = 0; d < max_word_size + 1 && d + i < sample_length; d++) {
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


/*
 * Calculates the probabilities for the given node, its childs and siblings
 */
void calculate_probabilities(Tree_node* node) {
  // defense against null node, prob-data
  if (node == NULL || node->prob_data == NULL) {
    return;
  }

  // calculates data for current node
  set_degrees_freedom(node);
  set_probability(node);

  // calculates for the other nodes
  calculate_probabilities(node->child);
  calculate_probabilities(node->sibling);
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
    df++;
    current_node = current_node->sibling;
  }
  node->prob_data->degrees_freedom = df;
}

/*
 * Calculates the probability for the current node.
 * If current node corresponds to "100", the corresponding probability is p(0|10)
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


/*
 * Method that calculates the Lw for the given node, its siblings and childs.
 */
void calculate_Lw(Tree_node* node) {
  // defense against null node, prob-data
  if (node == NULL || node->prob_data == NULL) {
    return;
  }

  double value = 1.0f;
  Tree_node* child = node->child;
  while (child != NULL) {
    value *= pow(child->prob_data->probability, child->prob_data->occurrences);
    child = child->sibling;
  }
  node->prob_data->Lw = value;


  calculate_Lw(node->child);
  calculate_Lw(node->sibling);
}
