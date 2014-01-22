/* Time-stamp: <2014/01/21 23:06:04 hutzpa [hutzpa] am> */

#include "glob.h"

/************************************************************* 
 *                 Global variables                          *
 *************************************************************/

Filename jobname;  // base name for all generated files

// These globals are command line parameters and are set once in main.c
char real_filename[PATH_MAX];
unsigned int depth = 5;
unsigned int number_resamples = 0;
unsigned int num_sizes = 5;
unsigned int seed = 0;
char *renewalstr = NULL;
double confid = .99;

char *alphabet = " 0123456789"; //  acceptable input chars, except for the initial one

char issymbol[256];  // will contain the index into alphabet of each char 
  
