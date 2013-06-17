/*
 * Definition of error codes and fatal error method.
 */
/* Time-stamp: <2013/05/04 22:18:10 hutzpa [hutzpa] am> */
#ifndef MESSAGES_H_
#define MESSAGES_H_



/*
 * The possible error codes
 */
enum Error_code {
  FILE_NOT_FOUND = 1,
  MISSING_FILENAME = 2,
  UNABLE_TO_RESAMPLE = 3
};

/*
 * This method shows a message according to the given error code, then exits the program.
 */
void fatal_error(enum Error_code code);

/*
 * Prints the usage manual and exits the program.
 */
void usage();

#endif /* MESSAGES_H_ */
