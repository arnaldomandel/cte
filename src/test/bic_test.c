/*
 * Test for the bic calculator
 */
#include "tree.h"
#include "bic.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Tree_node prob_root;
extern Tree_node bic_root;

/**
 * Test for the bic setup.
 */
int main(int argc, char** args) {
  char** samples = (char**) malloc(2 * sizeof(char*));
  samples[0] = "01010101";
  samples[1] = "";
  
  setup_BIC(samples, 2, prob_root, bic_root);
  print_tree(prob_root, 2);
  
  int i=0;

  // Verifying occurrences among the nodes
  {
    Tree_node node0  = get_create_child_node(prob_root, '0', PROB);
    Tree_node node00 = get_create_child_node(node0, '0', PROB);
    Tree_node node01 = get_create_child_node(node0, '1', PROB);
    Tree_node node1  = get_create_child_node(prob_root, '1', PROB);
    Tree_node node10 = get_create_child_node(node1, '0', PROB);
    Tree_node node11 = get_create_child_node(node1, '1', PROB);

    printf("%d\n",++i);
    // checking occurrences
    assert_equals_int(4, node0->p.occurrences,  "Number of occurrences wrong for node0\n");
    assert_equals_int(4, node01->p.occurrences, "Number of occurrences wrong for node01\n");
    assert_equals_int(0, node00->p.occurrences, "Number of occurrences wrong for node00\n");
    assert_equals_int(4, node1->p.occurrences,  "Number of occurrences wrong for node1\n");
    assert_equals_int(0, node11->p.occurrences, "Number of occurrences wrong for node11\n");
    assert_equals_int(3, node10->p.occurrences, "Number of occurrences wrong for node10\n");

    printf("%d\n",++i);
    // checking degrees of freedom
    assert_equals_int(1, node0->p.df,  "Number of degrees of freedom wrong for node0\n");
    assert_equals_int(1, node01->p.df,  "Number of degrees of freedom wrong for node01\n");
    assert_equals_int(0, node00->p.df,  "Number of degrees of freedom wrong for node00\n");
    assert_equals_int(1, node1->p.df,  "Number of degrees of freedom wrong for node1\n");
    assert_equals_int(1, node10->p.df,  "Number of degrees of freedom wrong for node10\n");
    assert_equals_int(0, node11->p.df,  "Number of degrees of freedom wrong for node1\n");

    printf("%d\n",++i);
    // checking probability
    assert_near_float(0.5f, node0->p.probability, "Probability wrong for node0\n");
    assert_near_float(0.0f, node00->p.probability, "Probability wrong for node00\n");
    assert_near_float(1.0f, node01->p.probability, "Probability wrong for node01\n");
    assert_near_float(0.5f, node1->p.probability, "Probability wrong for node1\n");
    assert_near_float(.75f, node10->p.probability, "Probability wrong for node10\n");
    assert_near_float(0.0f, node11->p.probability, "Probability wrong for node11\n");
  }
    printf("%d\n",++i);

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

  // checking most common word.
  {
    char* word = most_frequent_word(prob_root);
    assert_equals_int(2, strlen(word), "Most Frequent Word error\n");
    assert_equals_int('0', word[0], "Most Frequent Word error\n");
    assert_equals_int('1', word[1], "Most Frequent Word error\n");
  }
  printf("Tests completed successfully.\n");
}
