/*
 * Methods for calculating the bic tree given a cost.
 */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tau.h"
#include "tree.h"
#include "list.h"

/*
 * these variables are defined in bic_setup.c
 */
extern int max_word_size;
extern Tree_node* prob_root;
extern Tree_node* bic_root;
extern int sample_size;
extern double logN;


/*
 * Definition of functions that are implemented below.
 */
int node_depth(Tree_node* node);
Tree_node* get_prob_node(Tree_node* bic_node);
void add_selected_words_to_tau(Tree_node* node, Tau* tau);
char* recover_sufix(Tree_node* bic_node);
double recover_prob(char* sufix);
void get_v(Tree_node* bic_node, double c);
double node_part(Tree_node* node, Tree_node* prob_node, double c);
double children_part(Tree_node* node, double c);

/*
 * Calculates the BIC given the penalty c.
 */
Tau* calculate_BIC(double c) {
  get_v(bic_root->child, c);
  Tau* tau = new_Tau();
  tau->c = c;
  add_selected_words_to_tau(bic_root->child, tau);
  return tau;
}

/*
 * Calculates v values for a node, siblings and children, and decide if critical.
 */
void get_v(Tree_node* bic_node, double c) {
  if (bic_node == NULL || bic_node->bic_data == NULL) {
    return;
  }
  Tree_node* prob_node = get_prob_node(bic_node);

  // if node has no child, we use the node_part.
  if (node_depth(bic_node) == max_word_size || bic_node->child == NULL) {
    bic_node->bic_data->critical = 0;
    bic_node->bic_data->v = node_part(bic_node, prob_node, c);
    get_v(bic_node->sibling, c); // calculate siblings before returning
    return;
  }

  // calculate child, because it is needed for children_part calculation.
  get_v(bic_node->child, c);

  double n_p = node_part(bic_node, prob_node, c);
  double c_p = children_part(bic_node, c);

  // critical is the opposite of delta
  if (c_p > n_p) {
    bic_node->bic_data->critical = 0;
    bic_node->bic_data->v = c_p;
  } else {
    bic_node->bic_data->critical = 1;
    bic_node->bic_data->v = n_p;
  }

  // calculate siblings before returning
  get_v(bic_node->sibling, c);
}

/*
 * Calculates log( n^{-cdf(w)}Lw(X)) for a given node.
 * Either this or the value returned by children_part will become the v value.
 */
double node_part(Tree_node* bic_node, Tree_node* prob_node, double c) {
  return prob_node->prob_data->ell - c * prob_node->prob_data->degrees_freedom * logN;
}

/*
 * Calculates \sum_{a \in A} V_{aw}(X) for a given node.
 * Either this or the value returned by node_part will become the v value.
 */
double children_part(Tree_node* node, double c) {

  double part = 0;
  
  // for (Tree_node* current_child = node->child; current_child != NULL; current_child = current_child->sibling)
  ITERA(Tree_node*, current_child, node->child, sibling)
     part += current_child->bic_data->v;
  return part;
}


/*
 * Calculates the node depth.
 * If the given node is NULL, will return -1.
 * Return 0 for the root node.
 */
int node_depth(Tree_node* node) {
  int depth = -1;
  //for (Tree_node* current_node = node; current_node != NULL; current_node = current_node->parent)
  ITERA(Tree_node*, current_node, node, parent)
      depth++;
  return depth;
}

/*
 * Returns the prob_node which represents the same word as the given bic_node
 */
Tree_node* get_prob_node(Tree_node* bic_node) {
  Tree_node* prob_node = prob_root;
  
  for (Tree_node* parent_node = bic_node; parent_node != bic_root; parent_node = parent_node->parent)
    prob_node = get_child_node(prob_node, parent_node->symbol);
  
  return prob_node;
}


/*
 * Checks if the current node represents a word that must go to the final result.
 * Also checks children and siblings recursevely.
 */
void add_selected_words_to_tau(Tree_node* node, Tau* tau) {
  if(node == NULL) {
    return;
  }
  if (node->bic_data->critical) {
    // this node is selected, must add it to the tau and there is no need to check its children
    char* sufix = recover_sufix(node);
    double prob = recover_prob(sufix);
    insert_tau_item(tau, sufix, prob);
    tau->L += node->bic_data->v;
  } else {
    //this node is not selected, but it's children may
    add_selected_words_to_tau(node->child, tau);
  }

  // but we must always check siblings
  add_selected_words_to_tau(node->sibling, tau);
}

/*
 * Recovers the sufix for the given node
 */
char* recover_sufix(Tree_node* bic_node) {
  int length = node_depth(bic_node);
  char* sufix = (char*) malloc((length+1)* sizeof(char)); // +1 for the \0 terminator

  Tree_node* current_node = bic_node;
  for (int i = 0; i < length; i++) {
    sufix[i] = current_node->symbol;
    current_node = current_node->parent;
  }
  sufix[length] = '\0';
  return sufix;
}

/*
 * Recovers the probability for the given sufix.
 */
double recover_prob(char* sufix) {
  Tree_node* current_node = prob_root;
  for (int i = 0; sufix[i] != '\0'; i++) {
    current_node = get_child_node(current_node, sufix[i]);
  }
  return current_node->prob_data->probability;
}

/*
 * Returns the child of the given node which occurred the most.
 * If the depth of the node is max_word_size, then return the node.
 */
Tree_node* most_frequent_child(Tree_node* node) {
  if (node_depth(node) == max_word_size) {
    return node;
  }

  int max = 0;
  Tree_node* max_node;
  Tree_node* current_node = node->child;

  while (current_node != NULL) {
    Tree_node* max_child = most_frequent_child(current_node);
    if (max_child->prob_data != NULL && max_child->prob_data->occurrences > max) {
      max = max_child->prob_data->occurrences;
      max_node = max_child;
    }
    current_node = current_node->sibling;
  }
  return max_node;
}

/*
 * Return the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word() {
  char* word = (char*) malloc((max_word_size+1)*sizeof(char));
  Tree_node* max_node = most_frequent_child(prob_root);
  Tree_node* node = max_node;
  for (int i = 1; i <= max_word_size; i++) {
    word[max_word_size-i] = node->symbol;
    node = node->parent;
  }
  return word;
}

/*
 * Calculates Ltau on the current Prob Tree
 */
double L_tau(Tau* tau) {
  double value = 0;
  for(Tau_item* item = tau->item; item != NULL; item = item->next) {
    // go down the tree to find the correct node
    Tree_node* node = prob_root;
    for (char* symbol = item->string; symbol; symbol++)
      node = get_child_node(node, *symbol);

    if (node != NULL)
      value += node->prob_data->ell;
  }
  return value;
}
