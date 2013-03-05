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

#include "kmp.h"
#include "bic.h"
#include "messages.h"

// returns a random number between 0..(num-1)
#define random_num(num) (int)(rand() / (RAND_MAX+1.0) * num)



typedef struct omega Omega;

struct omega {
  char* word;
  Omega* next;
};

/*
 * Frees the memory allocated for the omega
 */
void free_omega(Omega* omega) {
  if (omega == NULL) return;
  Omega* next = omega->next;
  free(omega->word);
  free(omega);
  free_omega(next);
}

/*
 * Copies a range from the src string to a newly allocated one.
 */
char* copy_string(char* src, int start_index, int count) {
  char* string = (char*) malloc((count + 1) * sizeof(char));
  for (int i = 0; i < count; i++) {
    string[i] = src[start_index + i];
  }
  string[count] = '\0';
  return string;
}

/*
 * Creates the resamples to be used on the bootstrapping
 */
char** resample(char** samples, char* most_frequent_word, int size_resample1, int size_resample2, int number_of_resamples) {
  char** resamples = (char**) malloc((2*number_of_resamples)*sizeof(char*));
  Omega* omega_set = NULL;
  int word_size = strlen(most_frequent_word);

  // cycle through samples
  for(int i = 0; samples[i] != NULL; i++) {

    // will start searching after the first mf-word
    int begin = kmp(samples[i], 0, most_frequent_word) + word_size;

    while(1) {
      int next = kmp(samples[i], begin, most_frequent_word);
      if (next == -1) { //if there are no more matches, go to the next sample
        break;
      }
      // creates and addes the new item to omega
      Omega* new_item = (Omega*) malloc(sizeof(Omega));
      new_item->word = copy_string(samples[i], begin, next - begin);
      new_item->next = omega_set;
      omega_set = new_item;
      begin = next + word_size;
    }
  }

  int omega_size = 0;
  { // calculating the number os words in omega
    Omega* omega = omega_set;
    while (omega != NULL) {
      omega_size++;
      omega = omega->next;
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

    char* resamplei = (char*) malloc((size + 1)*sizeof(char));
    for (int i = 0; i < size + 1; i++) {
      resamplei[i] = '\0';
    }

    int current_size = 0;
    do {
      int selected = random_num(omega_size);
      Omega* omega = omega_set;
      for (int i = 0; i < selected; i++) {
        omega = omega->next;
      }
      strncat(resamplei, omega->word, size - current_size);
      current_size = strlen(resamplei);
    } while(current_size < size);
    resamples[i] = resamplei;
  }


  // the omega set wont be used anymore, so free its memory
  free_omega(omega_set);
  return resamples;
}

