/*
 * Implementation of methods declared in tao.h
 */

#include "tao.h"
#include <stdlib.h>

/*
 * Declaration of method defined below.
 */
Tao_item* new_tao_item(char* string, double prob);

/*
 * Inserts a new Tao_item in the given Tao.
 */
void insert_tao_item(Tao* tao, char* string, double prob) {
  Tao_item* new_item = new_tao_item(string, prob);
  new_item->next = tao->item;
  tao->item = new_item;
}

/*
 * instantiates a new Tao_item
 */
Tao_item* new_tao_item(char* string, double prob) {
  Tao_item* tao_item = malloc(sizeof(Tao_item));
  tao_item->string = string;
  tao_item->probability = prob;
  return tao_item;
}

Tao* new_Tao() {
  return malloc(sizeof(Tao));
}
