/*
 * Implements the selector of the champion set
 */

#include "tau.h"
#include "tree.h"
#include "bic.h"
#include "list.h"
#include "champion_set.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//remove
#include <stdio.h>

#define D(w)	((w)->b.dfsum - (w)->b.mate->p.df)
#define L(w)	((w)->b.lsum - (w)->b.mate->p.ell)

void get_next_min_node(Tree_node bic_tree, Tree_node *node);
extern int max_word_size;
extern double logN;

int champ_method = 1; // preference for the new method

/*
 * Method that return the champion tree set, based on prunning the initial tree one node at a time.
 * Assumes that the BIC calculator has already been set up.
 * List is in decreasing c order.
 * Makes no use of max_c or epsilon.
 */
Champion_item champion_set_prunning(Tree_node bic_tree) {

    int count = 0;

    // print_tree(bic_tree,"", max_word_size); /* DEBUG */
    
    
    MEM(Champion_item, champion_set, (Champion_item) malloc(sizeof(struct champion_item)));
    
    champion_set->tau = calculate_BIC(0, bic_tree);
    champion_set->next = NULL;
    Champion_item first_champ = champion_set;
    // pprint_Tau(champion_set->tau); /* DEBUG */
    
    
    /* Initialize lsum and dfsum */
    ITERA(Tau_item, t, champion_set->tau->item, next) {
	DEB("-----\n %s\n", t->string);
	
	Tree_node word_node = node_of_suffix(bic_tree, t->string);
	word_node->b.critical = 1;
	// print_node(word_node,"       ",word_node->depth); /* DEBUG */
	DEBS("");
	
	
	ITERA(Tree_node, node, word_node->parent, parent) {
	    //printf("%s ", recover_suffix(node)); /* DEBUG */
	    //print_node(node,"       ",node->depth); /* DEBUG */
	    /* 
             * if(!word_node->b.mate->p.ell  || !word_node->b.mate->p.df) { /\* DEBUG *\/
	     * 	DEBS("Alarm:");
	     * 	print_node(word_node->b.mate, "  ",0);
	     * }
             */
	    
	    node->b.lsum += word_node->b.mate->p.ell;
	    node->b.dfsum += word_node->b.mate->p.df;
	    node->b.critical = 0;
	    //printf("%s ", recover_suffix(node)); /* DEBUG */
	    //print_node(node,":::::::",node->depth); /* DEBUG */
	}
    }
    DEBS("Init");

    //print_tree(bic_tree, max_word_size);
    
    //print_Tau(last_champ->tau);
    //DEBS("printed");
    
    // when the last tau has only the epty string, we are finished
    while(champion_set->tau->item->string[0]){ /* tau is not just the root node */
	Tree_node minw = bic_tree;
	get_next_min_node(bic_tree, &minw);
	DEB("minw =  %p\n", minw);
	//DEB("eps %f\n",epsilon);
	
	//if(minc < .0000000000001) exit(1);

	// TODO: instead of just minw, iterate over all u st L(u)/D(u) == L(minw)/D(minw)
	/* prune at minw */
	minw->b.critical = 1;
	ITERA(Tree_node, node, minw->parent, parent) {
	    node->b.lsum -= L(minw);
	    node->b.dfsum -= D(minw);
	}
	

	// add a new item, at the end of the champion_set
	MEM(Champion_item, new_item, (Champion_item) malloc(sizeof(struct champion_item)));
	//MEMZ(Champion_item, new_item, 1);
	DEBS("got new");
	new_item->tau = new_Tau();
	DEBS("got new tau");
	
	add_selected_words_to_tau(bic_tree, new_item->tau);
	DEBS("added");
	
	new_item->tau->c = (L(minw)/D(minw)) / logN; 
	champion_set = champion_set->next = new_item;
	//new_item->next = champion_set;
	//champion_set = new_item;
	//pprint_Tau(champion_set->tau); /* DEBUG */
	DEB("Champion tree: %6d\n", ++count);
    }
    DEBS("");
    
    return first_champ;
}

void get_next_min_node(Tree_node node, Tree_node *nmin)
{
    //DEB("crit %d\n", node->b.critical);
    if(node->sibling)
	get_next_min_node(node->sibling, nmin);

    if( node->b.critical ) return;

    double c = L(node) / D(node);
    
    if( c > 0 && c < L(*nmin) / D(*nmin) ) {
	//*minc = c;
	*nmin = node;
	DEB("new: minc %f minw %p\n", L(node)/D(node), *nmin);
    }
    
    if(node->child)
	get_next_min_node(node->child, nmin);
}

/* 
 * void get_next_min_node(Tree_node node, Tree_node *nmin)
 * {
 *     //DEB("crit %d\n", node->b.critical);
 *     if(node->sibling)
 * 	get_next_min_node(node->sibling, nmin);
 * 
 *     if( node->b.critical ) return;
 * 
 *     if( D(node) == D(*nmin)  && L(node) < L(*nmin) )
 * 	*nmin = node;
 *     else if( D(node) < D(*nmin) ) {
 * //	c = ( L(node) - L(*nmin) ) / ( D(node) - D(*nmin) ); 
 * 	if( L(node) >= L(*nmin) ) {
 * 	    DEB("Alarm:  L(node) %f  L(*nmin) %f\n", L(node), L(*nmin)); 
 * 	}
 * 	else  {
 * 	    //\*minc = c;
 * 	    *nmin = node;
 * 	    DEB("new: minc %f minw %p\n", L(node)/D(node), *nmin);
 * 	}
 *     }
 *     
 *     if(node->child)
 * 	get_next_min_node(node->child, nmin);
 * }
 */

/* 
 * void get_minc(Tree_node bic_tree, double *minc, Tree_node *node)
 * {
 *     double c;
 *     DEB("crit %d\n", bic_tree->b.critical);
 *     
 *     if(!bic_tree->b.critical  && bic_tree->b.dfsum > bic_tree->b.mate->p.df) {
 * 	c = (bic_tree->b.lsum - bic_tree->b.mate->p.ell) /
 * 	    (bic_tree->b.dfsum - bic_tree->b.mate->p.df);
 * 	DEB("minc %f c %f minw %p\n", *minc, c, *node);
 * 	printf("%p %d  %f %f %d %d  %f\n", bic_tree, bic_tree->b.critical,bic_tree->b.lsum, bic_tree->b.mate->p.ell,
 * 	   bic_tree->b.dfsum, bic_tree->b.mate->p.df, c);
 * 	print_node(bic_tree, "",0);
 * 	
 * 	if(c < *minc) {
 * 	    *minc = c;
 * 	    *node = bic_tree;
 * 	    DEB("new: minc %f minw %p\n", *minc, *node);
 * 	    
 * 	}
 * 	  if(bic_tree->child)
 * 	    get_minc(bic_tree->child, minc, node);
 * 	  if(bic_tree->sibling)
 * 	    get_minc(bic_tree->sibling, minc, node);
 *     }
 * }
 */



/*
 * Method that return the champion tree set, based on the penalty function.
 * Assumes that the BIC calculator has already been set up.
 * List is in increasing c order.
 * since max_c is computed and epsilon is constant, there is no need for them
 */
Champion_item champion_set_minimizing(Tree_node bic_tree, double max_c, double epsilon) {
//    Tau root = calculate_BIC(max_c, bic_tree);
//    pprint_Tau(root);
//    exit(1);
    
    int count = 0;
    
    MEM(Champion_item, champion_set, (Champion_item) malloc(sizeof(struct champion_item)));
    
    champion_set->tau = calculate_BIC(0, bic_tree);
    champion_set->next = NULL;
    Champion_item first_champ = champion_set;

    /* Find the smallest c that gives a new tau */
    // when the last tau has only the epty string, we are finished
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

	champion_set = champion_set->next = new_item;
	//new_item->next = champion_set;
	//champion_set = new_item;
    }
    DEBS("");
    
    return first_champ;
}



Champion_item champion_set(Tree_node bic_tree, double max_c, double epsilon) 
{
    
    return champ_method ? champion_set_prunning(bic_tree)
                        : champion_set_minimizing(bic_tree, max_c, epsilon);
}
