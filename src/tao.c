/*
 * Implementation of methods declared in tao.h
 */

#include "tao.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

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

/*
 * Allocates and return a new Tao
 */
Tao* new_Tao() {
  Tao* tao = malloc(sizeof(Tao));
  tao->item = NULL;
  return tao;
}


/*
 * Frees the memory allocated by the given Tao and its items.
 */
void free_Tao(Tao* t) {
  Tao_item* item = t->item;
  while (item != NULL) {
    Tao_item* next_item = item->next;
    free(item);
    item = next_item;
  }
  free(t);
  t = NULL;
}


/*
 * returns the number os items in the given tao
 */
int items_in_Tao(Tao* t) {
  int items = 0;
  Tao_item* item = t->item;

  while (item != NULL) {
    items++;
    item = item->next;
  }
  return items;
}

/*
 * Verifies if both taos are equals, ie, have the same words.
 * Supposes there are no repeated words in the taos (as there shouldn't).
 * Ignores the c to which the Tao was calculated.
 */
int equals_Tao(Tao* t1, Tao* t2) {
  // if they dont have the same number of items, they are different
  if (items_in_Tao(t1) != items_in_Tao(t2)) {
    return 0;
  }

  Tao_item* item_t1 = t1->item;
  // for every item on t1, search for equivalent item in t2
  while (item_t1 != NULL) {
    Tao_item* item_t2 = t2->item;
    while(1) {
      if (item_t2 == NULL) {
        // could not find equivalent item in t2
        return 0;
      }
      if (strcmp(item_t1->string, item_t2->string) == 0) {
        //found equivalent item on t2, go to next item in t1
        break;
      }
      item_t2 = item_t2->next;
    }
    item_t1 = item_t1->next;
  }
  return 1;
}

/*
 * Prints the given Tao to stdout.
 */
void print_Tao(Tao* tao) {
  Tao_item* item = tao->item;
  printf("c=%f Tree=", tao->c);
  while (item != NULL) {
//    printf("%s(%f) ", item->string, item->probability);
    printf("%s ", item->string);
    item = item->next;
  }
  printf("\n");
}
