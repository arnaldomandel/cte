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
  }

  exit(code);
}

/*
 * Prints the usage manual.
 */
void usage() {

}
