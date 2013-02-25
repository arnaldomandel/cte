/*
 * Implements the selector of the champion set
 */

#include "tao.h"
#include "bic.h"
#include "champion_set.h"

#include <stdlib.h>

//remove
#include <stdio.h>


/*
 * Method that return the champion tree set.
 * Assumes that the BIC calculator has already been set up.
 */
Champion_item* champion_set(double max_c, double epsilon) {

  Tao* tao0 = calculate_BIC(0);
  Tao* root = calculate_BIC(max_c);

  Champion_item* champion_set = malloc(sizeof(Champion_item));
  champion_set->tao = root;

  // when tao0 equals to the last put tao in the champion set, all taos have been calculated
  while(!equals_Tao(tao0, champion_set->tao)) {
    double min = 0;
    double max = champion_set->tao->c;
    // invariable, !equals_Tao(calculate_BIC(min), calculate_BIC(max))

    //we do a binary search, while the difference between the extremes is greater than epsilon
    while (max - min > epsilon) {
      double mid = min + (max - min) / 2;

      Tao* mid_tao = calculate_BIC(mid);
      // if taos are equal, we bring the max closer to the min
      if (equals_Tao(champion_set->tao, mid_tao)) {
        max = mid;
      } else { // else since !equals_Tao(calculate_BIC(mid), calculate_BIC(max)), we bring min closer to max
        min = mid;
      }
      free_Tao(mid_tao);
    }

    // add a new item, as the head of the champion_set
    Champion_item* new_item = malloc(sizeof(Champion_item));
    new_item->tao = calculate_BIC(min);
    new_item->next = champion_set;
    champion_set = new_item;
  }
  return champion_set;
}
