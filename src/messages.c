/* Time-stamp: <2013/05/28 11:39:42 benavuya.ime.usp.br [benavuya] am> */

#include "glob.h"
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

  printf("Usage: cte -f file [OPTION]...\n       SMC estimation of context tree.\n\n");

  printf("  Option:                              meaning [default]\n");
  printf("    -f, --file=samples_file            file with the samples, mandatory\n");
  printf("    -d, --depth=n                      max size of the words to consider [5]\n");
  printf("    -j, --job-name=name                base name for files [constructed]\n");
  printf("    -K, --kept-trees=n                 keep n champion trees [all]\n");
  printf("    -R, --resample-method=x            resample method: e - ext, r - rand [e]\n");
  printf("    -n, --number-samples=n             number of resamples to create [10]\n");
  printf("    -s, --number-sizes=n               number of resample sizes [5]\n");
  printf("    -r, --renewal-string=TEXT          renewal string [M.F.W.]\n");
  printf("    -C, --confidence-level=f           confidence for rejecting 0 on regression [.99]\n");
  printf("    -m, --midpoint=a or m              choose average or median in the bootstrap [a]\n");
  printf("    -S, --seed=n                       pseudo-random generator seed [time]\n");
  printf("    -c, --champ-method=n               'o' chooses old method for champion trees\n");
}


