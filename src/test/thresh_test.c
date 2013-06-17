/*
 * Test for the bic calculater
 */
#include "read_file.h"
#include "assert.h"
#include "tree.h"
#include "tau.h"
#include "bic.h"
#include "champion_set.h"
#include "glob.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern Tree_node prob_root;
extern Tree_node bic_root;
extern double logN;

void get_T(Tree_node bic_node);

Vec get_Tvec(Tree_node node);
Vec get_cvec(Tree_node node);

Champion_item champion_set_from_vec(Vec c)
{
    Champion_item champion_set = (Champion_item) malloc(sizeof(struct champion_item));
    
    Tau tau0 = calculate_BIC(c->x[0], bic_root);
    champion_set->tau = tau0;
    champion_set->next = NULL;

    for(int i = 1; i < c->len; i++) {
	tau0 = calculate_BIC(c->x[i], bic_root);
	//if (!equals_Tau(tau0, champion_set->tau)) {
	{
	    
	    Champion_item new_item = (Champion_item) malloc(sizeof(struct champion_item));
	    new_item->tau = tau0;
	    new_item->next = champion_set;
	    champion_set = new_item;
	}
    }
    return champion_set;
    
}




 
/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
  char** samples = read_lines(argv[1]);
  int depth = strtod(argv[2], NULL);
  

  setup_BIC(samples, depth, prob_root, bic_root);
  print_tree(prob_root, depth);
  printf("----------------\n");
  
//  print_tree(bic_root, "");

  printf("\nlogN=%f\n\n",logN);
  
  // int n = size_of_sample();
  get_T(prob_root);
  print_tree(prob_root, depth);
    

  Vec c = get_Tvec(prob_root);
  print_Vec(c);
  
  sort_Vec(c);
  uniquefy_Vec(c);
  print_Vec(c);
  for(int i = 0; i < c->len; i++)
      c->x[i] /= logN;
  print_Vec(c);
  Champion_item champs = champion_set_from_vec(c);
  
  ITERA(Champion_item, cs, champs, next) {
      pprint_Tau(cs->tau);
      printf("\n");
  }
  
  return 0;
  
}
