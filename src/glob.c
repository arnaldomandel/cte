/* Time-stamp: <2013/05/28 11:39:43 benavuya.ime.usp.br [benavuya] am> */

#include "glob.h"

/************************************************************* 
 *                 Global variables                          *
 *************************************************************/

Filename jobname;  // base name for all generated files

// These globals are command line parameters and are set once in main.c
char real_filename[PATH_MAX];
unsigned int depth = 5;
unsigned int number_resamples = 10;
unsigned int num_sizes = 5;
unsigned int seed = 0;
char *renewalstr = NULL;
double confid = .99;

char *alphabet = " 0123456789"; //  acceptable input chars, except for the initial one

char issymbol[256];  // will contain the index into alphabet of each char 
  
