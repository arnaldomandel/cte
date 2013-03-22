/*
 * Test for the bic calculater
 */
#include "read_file.h"
#include "bic.h"
#include "assert.h"
#include "tree.h"
#include "champion_set.h"
#include "tau.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>


extern Tree_node* prob_root;
extern Tree_node* bic_root;

/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
  char** samples = read_lines(argv[1]);
  int depth = strtod(argv[2], NULL);
  double max_c = strtod(argv[3], NULL);
  double eps = strtod(argv[4], NULL);

  setup_BIC(samples, depth);
  // print_tree(prob_root, "");

  Champion_item* champs = champion_set(max_c, eps);

  ITERA(Champion_item*, c, champs, next) {
      pprint_Tau(c->tau);
      printf("\n");
  }
  
  
}
