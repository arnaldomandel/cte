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
#include "tree.h"
#include "list.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"

extern double logN;

// definition of functions implemented below
int accept_tree(Resamples resample, Tau current_tree, Tau next_tree);
Bivec delta_tau(Resamples res, Tau current_tree, Tau next_tree);
double threshold(int size);
void get_prob_trees(Resamples res, int depth) ;
void append_L_data(Tree_node node, Resamples res);
Bivec L_tau_vec(Tau tau, Resamples res);

double avg(double *v, int n)
{
    double a = 0;
    for (int i = 0; i < n; i++)
	a += v[i];
    return a / n;
}
double var(double *v, int n)
{
    double a = 0;
    for (int i = 0; i < n; i++)
	a += v[i]*v[i];
    return a / n - avg(v, n)*avg(v, n);
}
double stddev(double *v, int n)
{
    return sqrt(var(v, n));
}

// from student.c
void student(double *X, int Nx, double *Y, int Ny, double *both, double *left, double *right);

/*
 * Main method of the Bootstrap procedure.
 * Returns the tree that passes the t-student test
 */
Tau bootstrap(Resamples resample, Champion_item champion_set, int depth) {
    int number_of_trees = 0;
    Tau* champion_trees;
    Champion_item item;

    // complete the resample info
    get_prob_trees(resample, depth);
    
    append_L_data(resample->orig_tree, resample);
    
    
    // extract the Taus from the champion set to an array
    ITERA(, item, champion_set, next)
      number_of_trees++;
    MEM(,champion_trees, (Tau*) malloc(number_of_trees * sizeof(Tau)));
    item = champion_set;
    //      for (int i = number_of_trees - 1; i >= 0; i--) {
    for (int i = 0; i < number_of_trees; i++) {
	champion_trees[i] = item->tau;
	item = item->next;
    }
    
    DEB("champion_trees created %d trees\n",number_of_trees );
    



    for (int i = 0; i < number_of_trees - 1; i++) {
	//pprint_Tau(champion_trees[i]);
	//pprint_Tau(champion_trees[i+1]);
	DEB("tree %d\n", i);
	
	if(accept_tree(resample, champion_trees[i], champion_trees[i+1])) {
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
int accept_tree(Resamples resample, Tau current_tree, Tau next_tree) {

    double both, left, right;
    Bivec db;
    
    db = delta_tau(resample, current_tree, next_tree);
    
    
    /* 
     * // calculates each db[i]
     * for (int i = 0; i < number_resamples; i++) {
     * 	//db[0][i] = db[1][i] = 0.0;
     * 	sample[0] = resample[i];
     * 	db[0][i] = delta_tau(sample, current_tree, next_tree, depth);
     * 	sample[0] = resample[number_resamples + i];
     * 	db[1][i] = delta_tau(sample, current_tree, next_tree, depth);
     * }
     */

    printf("small: %9.6f %9.6f  large: %9.6f %9.6f   ", avg(db->small, resample->smallnum),stddev(db->small, resample->smallnum), avg(db->large, resample->largenum), stddev(db->large, resample->largenum));
    
    student(db->small, resample->smallnum, db->large, resample->largenum, &both, &left, &right);
    
    printf("b: %8.6f  l: %8.6f  r: %8.6f\n", both, left, right);
    
  /* 
   * double d_avg = 0.0;
   * double d_squared = 0.0;
   * for (int i = 0; i< number_resamples; i++) {
   *   d_avg += db[i];
   *   d_squared += db[i] * db[i];
   * }
   * d_avg = d_avg / number_resamples;
   * 
   * double Sd = sqrt((d_squared - d_avg * number_resamples)/(number_resamples - 1));
   * double t_b1 = d_avg / (Sd / sqrt(number_resamples));
   * 
   * 
   * // since the t-student test is not implemented, it displays the value which should be checked
   * printf("t_b1=%g tree=", t_b1);
   * print_Tau(current_tree);
   * printf("\n");
   */



  // the method should check if t_1b passes a t-student check and return proper value
  // currently it returns 0, so further calculations are done
  return 0;
}

/*
 * Calculates delta^(tau_i, tau_{i+1})(n,b)
 */
Bivec delta_tau(Resamples res, Tau current_tree, Tau next_tree) {
  
    Bivec L_tau1 = L_tau_vec(current_tree, res);
    Bivec L_tau2 = L_tau_vec(next_tree, res);

    for(int i = 0; i < res->smallnum; i++)
	L_tau1->small[i] = (L_tau1->small[i] - L_tau2->small[i]) / threshold(res->smallsize);
    for(int i = 0; i < res->largenum; i++)
	L_tau1->large[i] = (L_tau1->large[i] - L_tau2->large[i]) / threshold(res->largesize);
    
    free_bivec(L_tau2);
    

    //printf("%f %f %f\n", l_tau1, l_tau2, result);
  
/* this code is commented but may be uncommented for debugging purposes
   printf("l_tau1=%g\n", l_tau1);
   printf("l_tau2=%g\n", l_tau2);
   printf("/pow...=%g\n",pow(size_of_sample(), 0.9f));
   printf("log(l_tau1)=%g\n",log10(l_tau1));
   printf("log(l_tau2)=%g\n",log10(l_tau2));
   printf("delta log=%g\n",log10(l_tau1) - log10(l_tau2));
   printf("tudo=%g\n", result);
*/
    return L_tau1;
}


double threshold(int size)
{
    return pow(size, 0.9f);
}




/* 
 * Creates a prob tree for each sample
 */
void get_prob_trees(Resamples res, int depth) 
{
    char* sample[2];
    sample[1] = NULL;
    MEM(, res->small_trees, (Tree_node *) calloc(res->smallnum, sizeof(Tree_node)));
    MEM(, res->large_trees, (Tree_node *) calloc(res->largenum, sizeof(Tree_node)));
    for(int i = 0; i < res->smallnum; i++) {
	res->small_trees[i] = Tree_create(PROB);
	sample[0] = res->small[i];
	setup_BIC(sample, depth, res->small_trees[i], NULL);
	DEB("Small tree %4d: %4d\n", i, tree_size(res->small_trees[i]));
    }
    for(int i = 0; i < res->largenum; i++) {
	res->large_trees[i] = Tree_create(PROB);
	sample[0] = res->large[i];
	setup_BIC(sample, depth, res->large_trees[i], NULL);
	DEB("Large tree %4d: %4d\n", i, tree_size(res->large_trees[i]));
    }
}

	    






/* 
 * 
 * Calculate Lw for every word in a prob tree and every sample
 * completes the prob tree
 * 
 */
void append_L_data(Tree_node node, Resamples res) 
{
    char *w = word_of_node(node);
//    char *w = recover_suffix(node);

    node->p.L = new_bivec(res->smallnum, res->largenum);
    
    for(int i = 0; i < res->smallnum; i++)
	node->p.L->small[i] = Lw(w, res->small_trees[i]);
    for(int i = 0; i < res->largenum; i++)
	node->p.L->large[i] = Lw(w, res->large_trees[i]);
    free(w);
    if(node->child)
	append_L_data(node->child, res);
    if(node->sibling)
	append_L_data(node->sibling, res);
}

/* 
 * Bi-Vector of L_tau, indexed by samples, for a given champ tree tau 
 */
Bivec L_tau_vec(Tau tau, Resamples res)
{
    Bivec vec = new_bivec(res->smallnum, res->largenum);
    ITERA(Tau_item, item, tau->item, next) {
	Tree_node t = node_of_word(res->orig_tree, item->string);
	for(int i = 0; i < res->smallnum; i++)
	    vec->small[i] += t->p.L->small[i];
	for(int i = 0; i < res->largenum; i++)
	    vec->large[i] += t->p.L->large[i];
    }
    return vec;
}


