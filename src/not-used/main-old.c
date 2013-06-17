/**
 * Context Tree Estimation
 * Selects the variable length Markov chain (VLMC) using the smallest maximizer criterion (SMC).
 */

#include "read_file.h"
#include "bic.h"
#include "tau.h"
#include "glob.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"
#include "messages.h"

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
  if (argc != 6) {
    fatal_error(WRONG_ARGUMENTS);
  }
  char* filename = args[1];
  int depth = atoi(args[2]);
  //double max_c = strtod(args[3], NULL);
  //double epsilon = strtod(args[4], NULL);
  int size_resample1 = atoi(args[3]);
  int size_resample2 = atoi(args[4]);
  int number_resamples = atoi(args[5]);
  Tree_node prob_root = Tree_create(PROB);
  Tree_node bic_root = Tree_create(BIC);

  char** sample = read_lines(filename);

  
  setup_BIC(sample, depth, prob_root, bic_root); // pre calculations
  
  Champion_item champion_bics = champion_set(bic_root, Max_c(prob_root), Eps(prob_root)); // champions set calculation

  /* 
   * Champion_item champion_item = champion_bics;
   * 
   * while (champion_item != NULL) {
   *   printf("c=%f tree=[ ", champion_item->tau->c);
   *   print_Tau(champion_item->tau);
   *   printf("]\n");
   *   champion_item = champion_item->next;
   * }
   */



  char* word_most_frequent = most_frequent_word(prob_root);
  Resamples resamples = resample(sample, prob_root, word_most_frequent, size_resample1, size_resample2, number_resamples);

  free(word_most_frequent);
  free_lines(sample);


  printf("\nBootstrap starting\n");
  bootstrap(resamples, champion_bics, depth);

}
