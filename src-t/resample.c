/*
 * resample.c
 *
 *  Created on: Feb 27, 2013
 *      Author: onuki
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tree.h"
#include "kmp.h"
#include "bic.h"
#include "list.h"
#include "resample.h"
#include "messages.h"

// returns a random number between 0..(num-1)
#define random_num(num) (int)(rand() / (RAND_MAX+1.0) * num)

/*
 * Copies a range from the src string to a newly allocated one.
 */
/* 
 * char* clone_string(const char* src, const int start_index, const int count) {
 *     //MEM(char*, string, (char*) calloc((count + 1) ,  sizeof(char)));
 *   
 *     //strncpy(string, src + start_index, count);
 *     //return string;
 *     return strndup(src + start_index, count);
 * }
 */

/* 
 * Generates a sample of a given size concatenating random strings from a list
 */
char *gen_sample(char ** const list, const int listsize, const int samplesize)
{
    MEM(char*, sample, (char*) calloc(samplesize + 1, sizeof(char)));
    for(int current_size = 0; current_size < samplesize; current_size = strlen(sample)) 
	    strncat(sample, list[random_num(listsize)], samplesize - current_size);
    return sample;
}

/*
 * Creates the resamples to be used on the bootstrapping
 */
Resamples resample(char** samples, Tree_node prob_tree, char* most_frequent_word,
		int size_resample1, int size_resample2, int number_of_resamples) {
    // MEM(char**, resamples, (char**) malloc((2*number_of_resamples)*sizeof(char*)));
  
    int word_size = strlen(most_frequent_word);
    int num_pieces = node_of_word(prob_tree, most_frequent_word)->p.occurrences;
    MEM(char **, block_list, (char **) calloc(num_pieces, sizeof(char*)));
    int num_blocks = 0;
    Resamples resamples = (Resamples) malloc(sizeof(struct resamples));
    int count = 0;
    

    // cycle through samples
    for(int i = 0; samples[i] != NULL; i++) {

	// will start searching after the first mf-word
	// int begin = kmp(samples[i], 0, most_frequent_word) + word_size;

	char *begin = strstr(samples[i], most_frequent_word) + word_size;

	while(1) {
	    //int next = kmp(samples[i], begin, most_frequent_word);
	    char *next = strstr(begin, most_frequent_word);
	    //if (next == -1) { //if there are no more matches, go to the next sample
	    if (!next) { //if there are no more matches, go to the next sample
		break;
	    }
	    // creates and adds the new item to omega; includes the renewal
	    // block_list[num_blocks++] = clone_string(samples[i], begin, next - begin + word_size);
	    block_list[num_blocks++] = strndup(begin, (next - begin) + word_size);
	    begin = next + word_size;
	    DEB("Block: %7d\r", ++count);
	    //DEB("Block %6d: %s\n", ++count, block_list[num_blocks-1]);
	}
	DEBS("");
    }

    DEB("mfw: %s  occ: %d\n", most_frequent_word, num_blocks);
    
    /* 
     * for(int i=0; i<num_blocks;i++)
     * 	DEBS(block_list[i]);
     */
    
    if (num_blocks == 0) {
	fatal_error(UNABLE_TO_RESAMPLE);
    }

    
    // starting random number generator
    //srand((unsigned int)time(NULL));

    // storing the size of the original sample
    int sample_size = size_of_sample(prob_tree);

    resamples->smallnum = resamples->largenum = number_of_resamples;
    resamples->smallsize = sample_size * size_resample1 / 100;
    resamples->largesize = sample_size * size_resample2 / 100;
    resamples->orig_tree = prob_tree;
    MEM(,resamples->small, (char**) malloc((number_of_resamples)*sizeof(char*)));
    MEM(,resamples->large, (char**) malloc((number_of_resamples)*sizeof(char*)));

    count = 0;
    // generating all resamples
    for (int i = 0; i < number_of_resamples; i++) {
	resamples->small[i] = gen_sample(block_list, num_blocks, resamples->smallsize);
	resamples->large[i] = gen_sample(block_list, num_blocks, resamples->largesize);
	DEB("Resample: %5d\r", ++count);
    }
    DEBS("");

    // the block_list won't be used anymore, so free its memory
    for(int i = 0; i < num_blocks; i++)
	free(block_list[i]);
    free(block_list);

    return resamples;
}


