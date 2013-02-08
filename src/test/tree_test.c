/**
 * Test for tree.c
 */

#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Main method.
 * Read arguments, alphabet and samples.
 * Starts the BIC calculator.
 * Runs the champion trees selector.
 * Then runs the bootstrap method to return the selected Context Tree.
 */
int main(int argc, char** args) {
  Tree_node* root = get_tree_root();
  Tree_node* child1 = get_node_child(root, '1');
  Tree_node* child2 = get_node_child(root, '2');
  Tree_node* grandchild11 = get_node_child(child1, '1');
  get_node_child(child1, '2'); // we create this grandchild just to test

  Tree_node* test_get_child1 = get_node_child(root, '1');
  Tree_node* test_get_child2 = get_node_child(root, '2');

  if (child1 != test_get_child1) {
    printf("Error, child 1 was not properly retrieved.");
    exit(1);
  }

  if (child2 != test_get_child2) {
    printf("Error, child 2 was not properly retrieved.");
    exit(1);
  }

  Tree_node* current_node = get_node_child(root, '1');
  current_node = get_node_child(current_node, '1');

  if (current_node != grandchild11) {
    printf("Grandchild not properly retrieved.");
    exit(2);
  }

  printf("tests ran successfully\n");
  return 0;
}
