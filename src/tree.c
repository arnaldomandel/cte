/*
 * Implementation of the tree methods.
 */

#include "tree.h"

#include <stddef.h>
#include <stdlib.h>

Tree_node root;

// declaration of a function that is defined below
Tree_node* create_node_child(Tree_node* parent, char symbol);


/*
 * Searches for a node with the given symbol among the child of the given node.
 */
Tree_node* get_node_child(Tree_node* parent, char symbol) {
  if (parent->child == NULL) {
    parent->child = create_node_child(parent, symbol);
    return parent->child;
  }

  // walk through every child node, until we find a corresponding symbol, or stop at last node
  Tree_node* current_node = parent->child;
  while(current_node->data.symbol != symbol && current_node->sibling != NULL) {
    current_node = current_node->sibling;
  }

  // the correct node was found on the while loop
  if (current_node->data.symbol == symbol) {
    return current_node;
  }

  // the correct node was not found, and current node is the last node
  current_node->sibling = create_node_child(parent, symbol);
  return current_node->sibling;
}

/*
 * Returns the root of the tree.
 */
Tree_node* get_tree_root() {
  return &root;
}

/*
 * Creates a node and associates the given node as its parent and the given symbol as its own.
 */
Tree_node* create_node_child(Tree_node* parent, char symbol) {
  Tree_node* child = (Tree_node*)malloc(1*sizeof(Tree_node));
  child->data.symbol = symbol;
  child->parent = parent;
  return child;
}
