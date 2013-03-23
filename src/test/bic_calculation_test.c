/*
 * Test for the bic calculater
 */
#include "read_file.h"
#include "bic.h"
#include "assert.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>


extern Tree_node* prob_root;
extern Tree_node* bic_root;

/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
  char** samples = read_lines(argv[1]);
  double c = strtod(argv[2], NULL);

  setup_BIC(samples, 5);
  print_tree(prob_root, "");

  
  Tau* tao = calculate_BIC(c);
  print_tree(bic_root, "");

  pprint_Tau(tao);
  
}
