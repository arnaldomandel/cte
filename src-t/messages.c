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
  case MISSING_FILENAME:
    fprintf(stderr, "Missing filename.\n");
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

  printf("Usage: cte -f file [OPTION]...\n");

//  printf("cte <samples_file> <depth> <max_c> <epsilon> <size_resample1> <size_resample2> <number_of_resamples>\n\n");
  //printf("cte <samples_file> <depth> <size_resample1> <size_resample2> <number_of_resamples>\n\n");

  printf("  Option:                              meaning [default]\n");
  //printf("    -f, --file=samples_file          file with the samples, mandatory\n");
  printf("    -d, --depth=n                      max size of the words to consider [5]\n");
//  printf("<max_c> is a c that is used as the maximum cost, a Tau with this cost should be the root tree. ie. 3.5\n");
//  printf("<epsilon> the minimum difference between two costs, to calculate the champion set. ie. 0.01\n");
  printf("    -p, --small-sample-percentage=n    size of the small resamples (%%) [30]\n");
  printf("    -P, --large-sample-percentage=n    size of the large resamples (%%) [90]\n");
  printf("    -n, --number-samples=n             number of resamples to create [10]\n");
  printf("    -r, --renewal-string=TEXT          renewal string [M.F.W.]\n");
  printf("    -s, --seed=n                       pseudo-random generator seed [time]\n");
  printf("    -c, --champ-method=n               'o' chooses old method for champion trees\n");
  printf("    -C, --print-champs                 print all champion trees [no]\n");
}
