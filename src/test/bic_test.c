/*
 * Test for the bic calculater
 */
#include "tree.h"
#include "bic.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

extern Tree_node* prob_root;
extern Tree_node* bic_root;

/**
 * Test for the bic calculator.
 */
int main(int argc, char** args) {
  char** samples = malloc(2 * sizeof(char*));
  samples[0] = "01010101";
  setup_BIC("01", samples, 2);

  // Verifying occurrences among the nodes
  {
    Tree_node* node0  = get_create_node_child(prob_root, '0', PROB);
    Tree_node* node00 = get_create_node_child(node0, '0', PROB);
    Tree_node* node01 = get_create_node_child(node0, '1', PROB);
    Tree_node* node1  = get_create_node_child(prob_root, '1', PROB);
    Tree_node* node10 = get_create_node_child(node1, '0', PROB);
    Tree_node* node11 = get_create_node_child(node1, '1', PROB);

    // checking occurrences
    assert_equals_int(4, node0->prob_data->occurrences,  "Number of occurrences wrong for node0\n");
    assert_equals_int(4, node01->prob_data->occurrences, "Number of occurrences wrong for node01\n");
    assert_equals_int(0, node00->prob_data->occurrences, "Number of occurrences wrong for node00\n");
    assert_equals_int(4, node1->prob_data->occurrences,  "Number of occurrences wrong for node1\n");
    assert_equals_int(0, node11->prob_data->occurrences, "Number of occurrences wrong for node11\n");
    assert_equals_int(3, node10->prob_data->occurrences, "Number of occurrences wrong for node10\n");

    // checking degrees of freedom
    assert_equals_int(1, node0->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node0\n");
    assert_equals_int(1, node01->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node01\n");
    assert_equals_int(0, node00->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node00\n");
    assert_equals_int(1, node1->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node1\n");
    assert_equals_int(1, node10->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node10\n");
    assert_equals_int(0, node11->prob_data->degrees_freedom,  "Number of degrees of freedom wrong for node1\n");

    // checking probability
    assert_near_float(0.5f, node0->prob_data->probability, "Probability wrong for node0\n");
    assert_near_float(0.0f, node00->prob_data->probability, "Probability wrong for node00\n");
    assert_near_float(1.0f, node01->prob_data->probability, "Probability wrong for node01\n");
    assert_near_float(0.5f, node1->prob_data->probability, "Probability wrong for node1\n");
    assert_near_float(1.0f, node10->prob_data->probability, "Probability wrong for node10\n");
    assert_near_float(0.0f, node11->prob_data->probability, "Probability wrong for node11\n");
  }

  // verifying the BIC nodes do exist
  {
    if (bic_root->child->sibling == NULL) {
      fprintf(stderr, "One of the childs was not created.\n");
    }
    if (bic_root->child->child->sibling != NULL) {
      fprintf(stderr, "Extra child was created.\n");
    }
    if (bic_root->child->sibling->child->sibling != NULL) {
      fprintf(stderr, "Extra child was created.\n");
    }
  }

  printf("Tests completed successfully.\n");
}
