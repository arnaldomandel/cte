/*
 * resample.h
 *
 *  Created on: Feb 26, 2013
 *      Author: onuki
 */

#ifndef RESAMPLE_H_
#define RESAMPLE_H_

typedef struct resamples * Resamples;

struct resamples 
{
    char ** small;
    char ** large;
    Tree_node * small_trees;
    Tree_node * large_trees;
    int smallsize, smallnum, largesize, largenum;
    Tree_node orig_tree;
};


/*
 * Creates the resamples to be used on the bootstrapping
 */
Resamples resample(char** samples, Tree_node prob_tree, char* most_frequent_word, int size_resample1, int size_resample2, int number_of_resamples);

#endif /* RESAMPLE_H_ */
