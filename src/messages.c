#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

/*
 * Prints the message related to the error code and exits the program.
 */
void fatal_error(enum Error_code code) {

// prints the message related to the code
  switch (code) {
  case FILE_NOT_FOUND:
    fprintf(stderr, "The file containing the alphabet could not be found.\n");
    break;
  case WRONG_ARGUMENTS:
    fprintf(stderr, "Wrong number of arguments given.\n");
    usage();
    break;
  case UNABLE_TO_RESAMPLE:
    fprintf(stderr, "There was no object in the omega set.\nMost likely the most frequent word appeared only once on the/each sample.\n");
    break;
  }

  exit(code);
}

/*
 * Prints the usage manual.
 */
void usage() {

  printf("Usage: \n\n");

  printf("cte <samples_file> <depth> <max_c> <epsilon> <size_resample1> <size_resample2> <number_of_resamples>\n\n");

  printf("<samples_file> is the file with the samples ie. ./samples.txt\n");
  printf("<depth> is the depth of tree, or the size of the words to consider ie. 6\n");
  printf("<max_c> is a c that is used as the maximum cost, a Tau with this cost should be the root tree. ie. 3.5\n");
  printf("<epsilon> the minimum difference between two costs, to calculate the champion set. ie. 0.01\n");
  printf("<size_resample1> the size of the small resamples, in percentage ie. 10\n");
  printf("<size_resample2> the size of the big resamples, in percentage ie. 50\n");
  printf("<number_of_resamples> the number of resamples to create\n\n ");

}
