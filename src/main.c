/**
 * Context Tree Estimation
 * Selects the variable length Markov chain (VLMC) using the smallest maximizer criterion (SMC).
 */

#include "read_file.h"
#include "bic.h"
#include "tao.h"
#include "champion_set.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/*
 * Main method.
 * Read arguments, alphabet and samples.
 * Starts the BIC calculator.
 * Runs the champion trees selector.
 * Then runs the bootstrap method to return the selected Context Tree.
 */
int main(int argc, char** args) {
  char* filename = args[1];
  int depth = atoi(args[2]);
  double max_c = strtod(args[3], NULL);
  double epsilon = strtod(args[4], NULL);



  char** sample = read_lines(filename);
  setup_BIC(sample, depth);
  free_lines(sample);
  Champion_item* champion_bics = champion_set(max_c, epsilon);

  Champion_item* bic = champion_bics;
  while (bic != NULL) {
    print_Tao(bic->tao);
    bic = bic->next;
  }


}
