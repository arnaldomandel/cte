/*
 * bootstrap.c
 *
 *  Created on: Mar 1, 2013
 *      Author: onuki
 */

#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bic.h"
#include "tau.h"
#include "champion_set.h"

// definition of function implemented below
int accept_tree(char** resample, Tau* current_tree, Tau* next_tree, int depth, int number_resamples);
double delta_tau(char** sample, Tau* current_tree, Tau* next_tree, int depth);

/*
 * Main method of the Bootstrap procedure.
 * Returns the tree that passes the t-student test
 */
Tau* bootstrap(char** resample, Champion_item* champion_set, int depth, int number_resamples) {
  int number_of_trees = 0;
  Tau** champion_trees;
  { // extract the Taus from the champion set to an array
    Champion_item* item = champion_set;
    while (item != NULL) {
      number_of_trees++;
      item = item->next;
    }
    champion_trees = (Tau**) malloc(number_of_trees * sizeof(Tau*));
    item = champion_set;
    for (int i = number_of_trees - 1; i >= 0; i--) {
      champion_trees[i] = item->tau;
      item = item->next;
    }

  }

  for (int i = 0; i < number_of_trees - 1; i++) {
    if(accept_tree(resample, champion_trees[i], champion_trees[i+1], depth, number_resamples)) {
      free(champion_trees);
      return champion_trees[i];
    }
  }

  free(champion_trees);
  return champion_set->tau;
}


/*
 * Do the bootstrap calculation for a pair of trees.
 * Returns 0. It should return whether the tree passes the t-student test
 */
int accept_tree(char** resample, Tau* current_tree, Tau* next_tree, int depth, int number_resamples) {

  double db[number_resamples];
  char* sample[2];
  sample[1] = NULL;

  // calculates each db[i]
  for (int i = 0; i < number_resamples; i++) {
    db[i] = 0.0;
    sample[0] = resample[i];
    db[i] += delta_tau(sample, current_tree, next_tree, depth);
    sample[0] = resample[number_resamples + i];
    db[i] -= delta_tau(sample, current_tree, next_tree, depth);
  }


  double d_avg = 0.0;
  double d_squared = 0.0;
  for (int i = 0; i< number_resamples; i++) {
    d_avg += db[i];
    d_squared += db[i] * db[i];
  }
  d_avg = d_avg / number_resamples;

  double Sd = sqrt((d_squared - d_avg * number_resamples)/(number_resamples - 1));
  double t_b1 = d_avg / (Sd / sqrt(number_resamples));


  // since the t-student test is not implemented, it displays the value which should be checked
  printf("t_b1=%g tree=", t_b1);
  print_Tau(current_tree);
  printf("\n");



  // the method should check if t_1b passes a t-student check and return proper value
  // currently it returns 0, so further calculations are done
  return 0;
}

/*
 * Calculates delta^(tau_i, tau_{i+1})(n,b)
 */
double delta_tau(char** sample, Tau* current_tree, Tau* next_tree, int depth) {
  setup_BIC(sample, depth);
  double l_tau1 = L_tau(current_tree);
  double l_tau2 = L_tau(next_tree);
  // both l_tau are really close to 0 (10^-100 ~ 10^-300). So the logs below return -infinity
  double result = (log10(l_tau1) - log10(l_tau2))/pow(size_of_sample(), 0.9f);

/* this code is commented but may be uncommented for debugging purposes
  printf("l_tau1=%g\n", l_tau1);
  printf("l_tau2=%g\n", l_tau2);
  printf("/pow...=%g\n",pow(size_of_sample(), 0.9f));
  printf("log(l_tau1)=%g\n",log10(l_tau1));
  printf("log(l_tau2)=%g\n",log10(l_tau2));
  printf("delta log=%g\n",log10(l_tau1) - log10(l_tau2));
  printf("tudo=%g\n", result);
*/
  return result;
}
