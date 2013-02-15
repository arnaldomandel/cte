/*
 * Implementation of the tree methods.
 */

#include "tree.h"

#include <stddef.h>
#include <stdlib.h>

// declaration of a function that is defined below
Tree_node* create_node_child(Tree_node* parent, char symbol, int type);


/*
 * Searches for a node with the given symbol among the child of the given node.
 */
Tree_node* get_create_node_child(Tree_node* parent, char symbol, int type) {
  // parent node has no child, so we create its first child
  if (parent->child == NULL) {
    parent->child = create_node_child(parent, symbol, type);
    return parent->child;
  }

  // walk through every child node, until we find a corresponding symbol, or stop at last node
  Tree_node* current_node = parent->child;
  while(current_node->symbol != symbol && current_node->sibling != NULL) {
    current_node = current_node->sibling;
  }

  // the correct node was found on the while loop
  if (current_node->symbol == symbol) {
    return current_node;
  }

  // the correct node was not found, and current node is the last node
  current_node->sibling = create_node_child(parent, symbol, type);
  return current_node->sibling;
}

/*
 * Creates a node and associates the given node as its parent and the given symbol as its own.
 */
Tree_node* create_node_child(Tree_node* parent, char symbol, int type) {
  Tree_node* child = (Tree_node*)malloc(1*sizeof(Tree_node));
  child->symbol = symbol;
  child->parent = parent;

  // allocate memory for the data structure corresponding to the given type.
  if (type == BIC) {
    child->bic_data = malloc(sizeof(Bic_data));
  } else if (type == PROB){
    child->prob_data = malloc(sizeof(Prob_data));
  }

  return child;
}

/*
 * Frees the memory used by a node, its childs and siblings.
 */
void free_node(Tree_node* node) {
  if (node->bic_data != NULL) {
    free(node->bic_data);
  }
  if (node->prob_data != NULL) {
    free(node->prob_data);
  }
  if (node->child != NULL) {
    free_node(node->child);
  }
  if (node->sibling != NULL) {
    free_node(node->sibling);
  }
  free(node);
}
