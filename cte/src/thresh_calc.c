/*
 * Methods for calculating the relevant penalty costs
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "tau.h"
#include "tree.h"
#include "list.h"
#include "bic.h"
#include "vec.h"

/*
 * these variables are defined in bic_setup.c
 */
extern int max_word_size;
extern Tree_node prob_root;
extern Tree_node bic_root;
extern int sample_size;
extern double logN;

/*
 * Calculates T values for a tree, recursively
 */
void get_T(Tree_node prob_node) {
    if (!prob_node || !prob_node->prob_data)
	return;

    if (!prob_node->child || node_depth(prob_node) == max_word_size)
	prob_node->prob_data->T = 0;
    else {
	double lsum =  -prob_node->prob_data->ell, maxT = 0;
	int dfsum =  -prob_node->prob_data->degrees_freedom;
	
	ITERA(Tree_node, node, prob_node->child, sibling) {
	    get_T(node);
	    printf("%p %p [%d] %f\n", prob_node, node, node_depth(node), node->prob_data->T);
	    
	    lsum += node->prob_data->ell;
	    dfsum += node->prob_data->degrees_freedom;
	    /* 
             * if ( node->prob_data->T > maxT )
	     * 	maxT = node->prob_data->T;
             */
	    printf(": %p %f %f %d %f %d\n", node, maxT,lsum, dfsum,
		   node->prob_data->ell, node->prob_data->degrees_freedom);
	    
	}
	if (dfsum > 0) { // must be
	    double t = lsum / dfsum;
	    if ( t > maxT )
		maxT = t;
	}
	
	prob_node->prob_data->T = maxT;
    }
}


void rec_get_Tvec(Tree_node node, double **p) 
{
    if (!node) return;
    
    **p = node->prob_data->T;
    ++*p;
    
    rec_get_Tvec(node->child, p);
    rec_get_Tvec(node->sibling, p);
}


Vec get_Tvec(Tree_node node) 
{
    Vec v = create_Vec(tree_size(node));
    double *p = v->x;
    
    rec_get_Tvec(node, &p);
    v->len = v->alloc;
    return v;
}

Vec get_cvec(Tree_node node) 
{
    Vec c = get_Tvec(node);

    sort_Vec(c);
    uniquefy_Vec(c);
    for(int i = 0; i < c->len; i++)
	c->x[i] /= logN;
    return c;
}
