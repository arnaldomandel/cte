/*
 * Implementation of methods declared in tau.h
 */

#include "tau.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

/*
 * Declaration of method defined below.
 */
Tau_item* new_tau_item(char* string, double prob);

/*
 * Inserts a new Tau_item in the given Tau.
 */
void insert_tau_item(Tau* tau, char* string, double prob) {
  Tau_item* new_item = new_tau_item(string, prob);
  new_item->next = tau->item;
  tau->item = new_item;
}

/*
 * instantiates a new Tau_item
 */
Tau_item* new_tau_item(char* string, double prob) {
  Tau_item* tau_item = malloc(sizeof(Tau_item));
  tau_item->string = string;
  tau_item->probability = prob;
  return tau_item;
}

/*
 * Allocates and return a new Tau
 */
Tau* new_Tau() {
  Tau* tau = malloc(sizeof(Tau));
  tau->item = NULL;
  return tau;
}


/*
 * Frees the memory allocated by the given Tau and its items.
 */
void free_Tau(Tau* t) {
  Tau_item* item = t->item;
  while (item != NULL) {
    Tau_item* next_item = item->next;
    free(item);
    item = next_item;
  }
  free(t);
  t = NULL;
}


/*
 * returns the number os items in the given tau
 */
int items_in_Tau(Tau* t) {
  int items = 0;
  Tau_item* item = t->item;

  while (item != NULL) {
    items++;
    item = item->next;
  }
  return items;
}

/*
 * Verifies if both taus are equals, ie, have the same words.
 * Supposes there are no repeated words in the taus (as there shouldn't).
 * Ignores the c to which the Tau was calculated.
 */
int equals_Tau(Tau* t1, Tau* t2) {
  // if they dont have the same number of items, they are different
  if (items_in_Tau(t1) != items_in_Tau(t2)) {
    return 0;
  }

  Tau_item* item_t1 = t1->item;
  // for every item on t1, search for equivalent item in t2
  while (item_t1 != NULL) {
    Tau_item* item_t2 = t2->item;
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
 * Prints the given Tau to stdout.
 */
void print_Tau(Tau* tau) {
  Tau_item* item = tau->item;
  printf("c=%f Tree=", tau->c);
  while (item != NULL) {
//    printf("%s(%f) ", item->string, item->probability);
    printf("%s ", item->string);
    item = item->next;
  }
  printf("\n");
}
