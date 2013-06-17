/*
 * resample.c
 *
 *  Created on: Feb 27, 2013
 *      Author: onuki
 */
/* Time-stamp: <2013/05/26 23:36:01 hutzpa [hutzpa] am> */

#include "glob.h"
#include "tree.h"
#include "bic.h"
#include "resample.h"
#include "messages.h"

#include <math.h>

// returns a random number between 0..(num-1)
#define random_num(num) (int)(rand() / (RAND_MAX+1.0) * num)

/* 
 * Generates a sample of a given size concatenating random strings from a list
 */
char *gen_sample(char ** const list, const int listsize, const int samplesize)
{
    //DEB("gen_sample %p %d %d\n", list, listsize, samplesize);
    
    MEM(char*, sample, (char*) calloc(samplesize + 1, sizeof(char)));
    for(int current_size = 0; current_size < samplesize; current_size = strlen(sample)) 
	    strncat(sample, list[random_num(listsize)], samplesize - current_size);
    return sample;
}

/*
 * Creates the resamples to be used on the bootstrapping.  All resamples are independent.
 */
Resamples resample_rand(char** samples, Tree_node pre_tree, const char* split_word,
		int num_sizes, int number_of_resamples)
{
    // MEM(char**, resamples, (char**) malloc((2*number_of_resamples)*sizeof(char*)));
  
    int word_size = strlen(split_word);
    int num_pieces = node_of_word(pre_tree, split_word)->p.occurrences;
    MEM(char **, block_list, (char **) calloc(num_pieces, sizeof(char*)));
    int num_blocks = 0;
    Resamples resamples = (Resamples) malloc(sizeof(struct resamples));
    int count = 0, i;
    double scale = 1, scaling_factor;
    int minblock = 100000000, maxblock = 0;

    // cycle through samples
    for(i = 0; samples[i] != NULL; i++) {
	// partition into blocks
	// will start searching after the first split-word
	char *begin = strstr(samples[i], split_word) + word_size;

	while(1) {
	    char *next = strstr(begin, split_word);
	    if (!next) { //if there are no more matches, go to the next sample
		break;
	    }
	    int blsiz = (next - begin) + word_size;
	    block_list[num_blocks++] = strndup(begin, blsiz);
	    begin = next + word_size;
	    if(blsiz < minblock) minblock = blsiz;
	    else if(blsiz > maxblock) maxblock = blsiz;
	    DEB("Block: %7d\r", ++count);
	    //DEB("Block %6d: %s\n", ++count, block_list[num_blocks-1]);
	}
	DEBS("");
    }

    mess("Split word: %s  Num blocks: %d  Min block: %d  Max block: %d\n", split_word, num_blocks, minblock, maxblock);
    
    /* 
     * for(int i=0; i<num_blocks;i++)
     * 	DEBS(block_list[i]);
     */
    
    if (num_blocks < MIN_BLOCKS) {
	fatal_error(UNABLE_TO_RESAMPLE);
    }

    // storing the adjusted size of the original sample
    int sample_size = size_of_sample(pre_tree) * SMALL_SAMPLE;

    mess("Sample size: %10d ", pre_tree->p.occurrences);
    
    resamples->num_sizes = num_sizes;
    resamples->num_resamples = number_of_resamples;
    resamples->orig_tree = pre_tree;
    scaling_factor = exp( log(LARGE_SAMPLE/SMALL_SAMPLE) / (num_sizes - 1) );
    mess("Scaling factor %f\nSample sizes:\n", scaling_factor);

	
    for(i = 0; i < num_sizes; i++) {
	resamples->s[i].size = (int) sample_size * scale;
	mess("%9d\n", resamples->s[i].size);
	MEM(,resamples->s[i].sample, (char**) malloc((number_of_resamples)*sizeof(char*)));
	scale *= scaling_factor;
    }
    mesg("");
    
    DEBS("Start resample vector");
    
    count = 0;
    // generating all resamples
    for (int j = 0; j < resamples->num_sizes; j++) {
	for (i = 0; i < number_of_resamples; i++) {
	    mess("Resample: %5d %4d %4d %6d\r", ++count, i, j, resamples->s[j].size);
	    resamples->s[j].sample[i] = gen_sample(block_list, num_blocks, resamples->s[j].size);
	}
    }
    
    DEBS("");

    // the block_list won't be used anymore, so free its memory
    for(i = 0; i < num_blocks; i++)
    	free(block_list[i]);
    free(block_list);

    return resamples;
}


/*
 * Creates the resamples to be used on the bootstrapping.
 * Largest size resamples are independent, smaller resamples are truncations of the large ones.
 */
Resamples resample_ext(char** samples, Tree_node pre_tree, const char* split_word,
		       int num_sizes, int number_of_resamples)
{
    // MEM(char**, resamples, (char**) malloc((2*number_of_resamples)*sizeof(char*)));
  
    int word_size = strlen(split_word);
    int num_pieces = node_of_word(pre_tree, split_word)->p.occurrences;
    MEM(char **, block_list, (char **) calloc(num_pieces, sizeof(char*)));
    int num_blocks = 0;
    Resamples resamples = (Resamples) malloc(sizeof(struct resamples));
    int count = 0, i;
    double scale = 1, scaling_factor;
    int minblock = 100000000, maxblock = 0;

    // cycle through samples
    for(i = 0; samples[i] != NULL; i++) {
	// partition into blocks
	// will start searching after the first split-word
	char *begin = strstr(samples[i], split_word) + word_size;

	while(1) {
	    char *next = strstr(begin, split_word);
	    if (!next) { //if there are no more matches, go to the next sample
		break;
	    }
	    int blsiz = (next - begin) + word_size;
	    block_list[num_blocks++] = strndup(begin, blsiz);
	    begin = next + word_size;
	    if(blsiz < minblock) minblock = blsiz;
	    else if(blsiz > maxblock) maxblock = blsiz;
	    DEB("Block: %7d\r", ++count);
	    //DEB("Block %6d: %s\n", ++count, block_list[num_blocks-1]);
	}
	DEBS("");
    }

    mess("Split word: %s  Num blocks: %d  Min block: %d  Max block: %d\n", split_word, num_blocks, minblock, maxblock);
    
    /* 
     * for(int i=0; i<num_blocks;i++)
     * 	DEBS(block_list[i]);
     */
    
    if (num_blocks < MIN_BLOCKS) {
	fatal_error(UNABLE_TO_RESAMPLE);
    }

    // storing the adjusted size of the original sample
    int sample_size = size_of_sample(pre_tree) * SMALL_SAMPLE;

    mess("Sample size: %10d ", pre_tree->p.occurrences);
    
    resamples->num_sizes = num_sizes;
    resamples->num_resamples = number_of_resamples;
    resamples->orig_tree = pre_tree;
    scaling_factor = exp( log(LARGE_SAMPLE/SMALL_SAMPLE) / (num_sizes - 1) );
    mess("Scaling factor %f\nSample sizes:\n", scaling_factor);

	
    for(i = 0; i < num_sizes; i++) {
	resamples->s[i].size = (int) sample_size * scale;
	mess("%9d\n", resamples->s[i].size);
	MEM(,resamples->s[i].sample, (char**) malloc((number_of_resamples)*sizeof(char*)));
	scale *= scaling_factor;
    }
    mesg("");
    
    DEBS("Start resample vector");
    
    count = 0;
    // generating all resamples
    for (i = 0; i < number_of_resamples; i++) {
	// Generate the larger one
	resamples->s[resamples->num_sizes-1].sample[i] =
	    gen_sample(block_list, num_blocks, resamples->s[resamples->num_sizes-1].size);
	count += num_sizes;
	mess("Resample: %5d\r", count);
	for (int j = 0; j < resamples->num_sizes-1; j++) {
	    // Truncate to get the smaller ones
	    resamples->s[j].sample[i] =
		strndup(resamples->s[resamples->num_sizes-1].sample[i], resamples->s[j].size);
	}
    }
    
    DEBS("");

    // the block_list won't be used anymore, so free its memory
    for(i = 0; i < num_blocks; i++)
    	free(block_list[i]);
    free(block_list);

    return resamples;
}    
