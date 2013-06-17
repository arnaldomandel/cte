/*
 * Implements the selector of the champion set
 */

#include "tau.h"
#include "bic.h"
#include "glob.h"
#include "champion_set.h"

#include <stdlib.h>
#include <stddef.h>

//remove
#include <stdio.h>

/*
 * Method that return the champion tree set.
 * Assumes that the BIC calculator has already been set up.
 * List is in decreasing c order.
 */
Champion_item champion_set(Tree_node bic_tree, double max_c, double epsilon) {
//    Tau root = calculate_BIC(max_c, bic_tree);
//    pprint_Tau(root);
//    exit(1);
    
    int count = 0;
    
    MEM(Champion_item, champion_set, (Champion_item) malloc(sizeof(struct champion_item)));
    
    champion_set->tau = calculate_BIC(0, bic_tree);
    champion_set->next = NULL;

    /* Find the smallest c that gives a new tau */
    // when tau0 equals to the last put tau in the champion set, all taus have been calculated
//    while(!equals_Tau(root, champion_set->tau)) {
    while(*champion_set->tau->item->string) {
	double min = champion_set->tau->c;
	double max = max_c;
	// invariant, !equals_Tau(calculate_BIC(min), calculate_BIC(max))

	//we do a binary search, while the difference between the extremes is greater than epsilon
	while (max - min > epsilon) {
	    // double mid = min + (max - min) / 2;  // nonsense here
	    double mid = (max + min) / 2;

	    Tau mid_tau = calculate_BIC(mid, bic_tree);
	    // if taus are equal, we bring the max closer to the min
	    if (equals_Tau(champion_set->tau, mid_tau)) {
		min = mid;
	    } else { // else since !equals_Tau(calculate_BIC(mid), calculate_BIC(max)), we bring min closer to max
		max = mid;
	    }
	    free_Tau(mid_tau);
	}

	// add a new item, as the head of the champion_set
	MEM(Champion_item, new_item, (Champion_item) malloc(sizeof(struct champion_item)));
	
	new_item->tau = calculate_BIC(max, bic_tree);
    
	DEB("Champion tree: %6d\r", ++count);
	
	/* 
         * //debugging
	 * printf("\n------------\n");
	 * print_tree(bic_root, "", 8);
	 * pprint_Tau(new_item->tau);
         */
    
	new_item->next = champion_set;
	champion_set = new_item;
    }
    DEBS("");
    
    return champion_set;
}

/* 
 * Champion_item champion_set(double max_c, double epsilon) {
 * 
 *   Tau tau0 = calculate_BIC(0);
 *   Tau root = calculate_BIC(max_c);
 * 
 *   Champion_item champion_set = (Champion_item) malloc(sizeof(struct champion_item));
 *   champion_set->tau = root;
 *   champion_set->next = NULL;
 * 
 *   // when tau0 equals to the last put tau in the champion set, all taus have been calculated
 *   while(!equals_Tau(tau0, champion_set->tau)) {
 *     double min = 0;
 *     double max = champion_set->tau->c;
 *     // invariant, !equals_Tau(calculate_BIC(min), calculate_BIC(max))
 * 
 *     //we do a binary search, while the difference between the extremes is greater than epsilon
 *     while (max - min > epsilon) {
 * 	// double mid = min + (max - min) / 2;  // nonsense here
 * 	double mid = (max + min) / 2;
 * 
 *       Tau mid_tau = calculate_BIC(mid);
 *       // if taus are equal, we bring the max closer to the min
 *       if (equals_Tau(champion_set->tau, mid_tau)) {
 *         max = mid;
 *       } else { // else since !equals_Tau(calculate_BIC(mid), calculate_BIC(max)), we bring min closer to max
 *         min = mid;
 *       }
 *       free_Tau(mid_tau);
 *     }
 * 
 *     // add a new item, as the head of the champion_set
 *     Champion_item new_item = (Champion_item) malloc(sizeof(struct champion_item));
 *     new_item->tau = calculate_BIC(min);
 *     
 * 
 *     printf("\n------------\n");
 *     print_tree(bic_root, "", 8);
 *     pprint_Tau(new_item->tau);
 *     
 *     new_item->next = champion_set;
 *     champion_set = new_item;
 *   }
 *   return champion_set;
 * }
 */

