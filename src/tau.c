/*
 * Implementation of methods declared in tau.h
 */

#include "tau.h"
#include "list.h"
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

/* 
 * void insert_tau_item(Tau* tau, char* string, double prob) {
 *   Tau_item* new_item = new_tau_item(string, prob);
 *   new_item->next = tau->item;
 *   tau->item = new_item;
 * }
 */

/*
 * Inserts a new Tau_item at the end of the given Tau, so that strings are in revlex order
 */
void insert_tau_item(Tau* tau, char* string, double prob) {
    Tau_item* new_item = new_tau_item(string, prob);
    if(tau->item)
	tau->last = tau->last->next = new_item;
    else
	tau->last = tau->item = new_item;
}


/*
 * instantiates a new Tau_item
 */
Tau_item* new_tau_item(char* string, double prob) {
  Tau_item* tau_item = (Tau_item*) malloc(sizeof(Tau_item));
  tau_item->string = string;
  tau_item->probability = prob;
  tau_item->next = NULL;
  return tau_item;
}

/*
 * Allocates and return a new Tau
 */
Tau* new_Tau() {
  Tau* tau = (Tau*) malloc(sizeof(Tau));
  tau->last = tau->item = NULL;
  tau->L = 0;
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

  // while (item != NULL) {
  ITERA(Tau_item*, item, t->item, next)
    items++;

  return items;
}

/*
 * Verifies if both taus are equals, ie, have the same words.
 * Supposes there are no repeated words in the taus (as there shouldn't).
 * Ignores the c to which the Tau was calculated.
 */
/*
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
*/

/* 
 * Verifies whether the two taus are equal.  Since tau is an ordered 
 * list of strings, a simple element by element comparison suffices.
 */
int equals_Tau(Tau* t1, Tau* t2) {
    Tau_item *p1, *p2;
    for(p1=t1->item, p2=t2->item; p1 && p2; p1=p1->next, p2=p2->next)
	if(strcmp(p1->string, p2->string))
	    return 0;  // they differ
    return !p1 && !p2;  // true if at the end of both
}

     

/*
 * Prints the given Tau to stdout.
 */
void print_Tau(Tau* tau) {
    // for (Tau_item* item = tau->item; item != NULL; item = item->next)
    ITERA(Tau_item*, item, tau->item, next)
	printf("%s ", item->string);
//    printf("%s(%f) ", item->string, item->probability);
}

void pprint_Tau(Tau* tau) {
    Tau_item* item;
    int siz = 0, l;

    // finds max length for pretty printing suffixes 
    ITERA(, item, tau->item, next)
	if ( siz < ( l = strlen(item->string) ) )
	     siz = l;

    printf("[%f]\n", tau->L);
    // for (item = tau->item; item != NULL; item = item->next)  
    ITERA(, item, tau->item, next)
	printf("%*s\n", siz, item->string);
}

	     
