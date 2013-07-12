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
#include "messages.h"

extern Tree_node prob_root;

// returns a random number between 0..(num-1)
#define random_num(num) (int)(rand() / (RAND_MAX+1.0) * num)

/*
 * Copies a range from the src string to a newly allocated one.
 */
char* copy_string(char* src, int start_index, int count) {
  char* string = (char*) malloc((count + 1) * sizeof(char));
  strncpy(string, src + start_index, count);
  return string;
}

/*
 * Creates the resamples to be used on the bootstrapping
 */
char** resample(char** samples, char* most_frequent_word, int size_resample1, int size_resample2, int number_of_resamples) {
  char** resamples = (char**) malloc((2*number_of_resamples)*sizeof(char*));
  int word_size = strlen(most_frequent_word);
  int num_pieces = node_of_word(prob_root, most_frequent_word)->prob_data->occurrences;
  char **omega_set = (char **) calloc(num_pieces, sizeof(char*));
  int omega_size = 0;
  

  // cycle through samples
  for(int i = 0; samples[i] != NULL; i++) {

    // will start searching after the first mf-word
    int begin = kmp(samples[i], 0, most_frequent_word) + word_size;

    while(1) {
      int next = kmp(samples[i], begin, most_frequent_word);
      if (next == -1) { //if there are no more matches, go to the next sample
        break;
      }
      // creates and adds the new item to omega; icludes the renewal
      omega_set[omega_size++] = copy_string(samples[i], begin, next - begin + word_size);
      begin = next + word_size;
    }
  }

  if (omega_size == 0) {
    fatal_error(UNABLE_TO_RESAMPLE);
  }

  // starting random number generator
  srand((unsigned int)time(NULL));

  // storing the size of the original sample
  int sample_size = size_of_sample();

  // generating all resamples
  for (int i = 0; i < 2 * number_of_resamples; i++) {
    int size;
    if (i < number_of_resamples) { // the first half of resamples will have the smallest size, the other half the bigger size
      size = sample_size * size_resample1 / 100;
    } else {
      size = sample_size * size_resample2 / 100;
    }

    char* resamplei = (char*) calloc(size + 1, sizeof(char));

    for(int current_size = 0; current_size < size; current_size = strlen(resamplei)) 
	strncat(resamplei, omega_set[random_num(omega_size)], size - current_size);
    resamples[i] = resamplei;
  }


  // the omega set wont be used anymore, so free its memory
  free(omega_set);
  return resamples;
}

