/*
 * resample.h
 *
 *  Created on: Feb 26, 2013
 *      Author: onuki
 */

/* Time-stamp: <2013/08/30 15:58:25 benavuya.ime.usp.br [benavuya] am> */
#ifndef RESAMPLE_H_
#define RESAMPLE_H_

#define NUM_SIZES 20

// Fractions of the big text; numbers chosen so that there is a factor of 4
#define LARGE_SAMPLE .92
#define SMALL_SAMPLE .23

// Minimum number of blocks for an acceptable bootstrap
#define MIN_BLOCKS 100

typedef struct resamples * Resamples;

struct samples 
{
    int size;
    char **sample;       // a vector of strings, all of length size
    Tree_node * tree;   // a vector of prob trees; those vectors are parallel, length is num_resamples
};
    
/* 
 * In a struct resamples, each sample is described by two indices
 * the first refers to the size
 * the second indexes the sample within the size
 */
struct resamples 
{
    struct samples s[NUM_SIZES];
    int num_sizes;
    int num_resamples;
    Tree_node orig_tree;  // prob tree
};


/*
 * Creates the resamples to be used on the bootstrapping
 */
Resamples resample_rand(char** samples, Tree_node prob_tree, const char* split_word,
		   int num_sizes, int number_of_resamples);
Resamples resample_ext(char** samples, Tree_node pre_tree, const char* split_word,
		       int num_sizes, int number_of_resamples);

#endif /* RESAMPLE_H_ */
