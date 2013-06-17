/*
 * Test for the bic calculater
 */
#include "read_file.h"
#include "bic.h"
#include "assert.h"
#include "tree.h"
#include "champion_set.h"
#include "tau.h"
#include "glob.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern Tree_node prob_root;
extern Tree_node bic_root;
extern double logN;
extern int max_word_size;

Tree_node most_frequent_leaf(Tree_node node);
Tree_node most_frequent_child(Tree_node node);


/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
  char** samples = read_lines(argv[1]);
  int depth = strtod(argv[2], NULL);

  

  setup_BIC(samples, depth, prob_root, bic_root);
  

  print_tree(prob_root, -(depth+1));

  printf("\n-----------------\n");
  
  printf("Most: %p\n", most_frequent_leaf(prob_root));
  printf("most: %p\n", most_frequent_child(prob_root));
  printf("\n-----------------\n");
  
  for(Tree_node t = next_node_depth(prob_root, max_word_size); t; t = next_node_depth(t, max_word_size))
      printf("%p\n",t);
  
}
