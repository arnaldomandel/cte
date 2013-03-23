/**
 * Test for tree.c
 */

#include "../tree.h"
#include "assert.h"

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
  Tree_node* root = (Tree_node*) malloc(sizeof(Tree_node));
  Tree_node* child1 = get_create_child_node(root, '1', PROB);
  Tree_node* child2 = get_create_child_node(root, '2', PROB);
  Tree_node* grandchild11 = get_create_child_node(child1, '1', PROB);
  get_create_child_node(child1, '2', PROB); // we create this grandchild just to test

  Tree_node* test_get_child1 = get_create_child_node(root, '1', PROB);
  Tree_node* test_get_child2 = get_create_child_node(root, '2', PROB);


  assert_equals(child1, test_get_child1, "Error, child 1 was not properly retrieved.");

  assert_equals(child2, test_get_child2,"Error, child 2 was not properly retrieved.");

  assert_equals(child1->parent, root, "Parent for child1 not properly assigned.");

  assert_equals(child2->parent, root, "Parent for child2 not properly assigned.");

  Tree_node* current_node = get_create_child_node(root, '1', PROB);
  current_node = get_create_child_node(current_node, '1', PROB);

  assert_equals(current_node, grandchild11, "Grandchild not properly retrieved.");

  assert_equals(grandchild11->parent, child1, "Parent for grandchild11 not properly assigned.");

  printf("tests ran successfully\n");
  return 0;
}
