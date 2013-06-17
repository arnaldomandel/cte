/**
 * Context Tree Estimation
 * Selects the variable length Markov chain (VLMC) using the smallest maximizer criterion (SMC).
 */

#include "read_file.h"
#include "bic.h"
#include "tau.h"
#include "list.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"
#include "messages.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <err.h>
#include <getopt.h>

extern int champ_method;
extern double logN, scale;


#define IOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		e = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = atoi(optarg);\
	    break
#define FOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		e = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = atof(optarg);\
	    break
#define SOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		e = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = strdup(optarg);		\
	    break
#define NOPT(c,var)	 	case c:\
	      var = 1;		\
	    break
		
/*
 * Main method.
 * Read arguments, alphabet and samples.
 * Starts the BIC calculator.
 * Runs the champion trees selector.
 * Then runs the bootstrap method to return the selected Context Tree.
 */
int main(int argc, char** argv) {
    /* Parameters and defaults */
    char* filename = NULL;
    unsigned int depth = 5;
    unsigned int size_resample_small = 30;
    unsigned int size_resample_large = 90;
    unsigned int number_resample_small = 10;
    unsigned int number_resample_large = 10;
    char *renewalstr = NULL;
    unsigned int seed = 0;
    char *c_method = NULL;
    int print_champs = 0;

    /* Process options */
    int c, e = 0;
    while (1) {
	static struct option long_options[] =  {
	    {"file",  required_argument, 0, 'f'},
	    {"depth",  required_argument, 0, 'd'},
	    {"small-sample-percentage",  required_argument, 0, 'p'},
	    {"large-sample-percentage",    required_argument, 0, 'P'},
	    {"number-samples",    required_argument, 0, 'n'},
	    {"number-large-samples",    required_argument, 0, 'N'},
	    {"renewal-string",    required_argument, 0, 'r'},
	    {"seed",    required_argument, 0, 's'},
	    {"scale",    required_argument, 0, 'k'},
	    {"champ-method",    required_argument, 0, 'c'},
	    {"print-champs", no_argument, 0, 'C'},
	    {0, 0, 0, 0}
	};
	int option_index = 0;
	c = getopt_long (argc, argv, "f:d:p:P:n:N:r:s:c:k:C", long_options, &option_index);
     
	if ( c == -1)
	    break;
	switch ( c ) {
	    SOPT('f', "filename", filename);
	    IOPT('d', "depth", depth);
	    IOPT('p', "small sample percentage", size_resample_small);
	    IOPT('P', "large sample percentage", size_resample_large);
	    IOPT('n', "small resample number", number_resample_small);
	    IOPT('N', "large resample number", number_resample_large);
	    IOPT('s', "seed", seed);
	    FOPT('k', "scale", scale);
	    SOPT('r', "renewal string", renewalstr);
	    SOPT('c', "champ method", c_method);
	    NOPT('C', print_champs);
	case '?':
	    e = 1;
	}
    }
    DEB("file %s\ndepth %d\nsmall p %d\nlarge p %d\nsmall num %d\nlarge num %d\nseed %d\nren %s\n",
	   filename,depth,size_resample_small,size_resample_large,number_resample_small,number_resample_large,seed,renewalstr);
    champ_method = !c_method || c_method[0] != 'o';
    
    if(e){
	usage();
	exit(0);
    }
    if (optind < argc && !filename)
	filename = strdup(argv[optind]);

    if(!filename)
	fatal_error(MISSING_FILENAME);
    
    Tree_node prob_root = Tree_create(PROB);
    Tree_node bic_root = Tree_create(BIC);
    
    char** sample = read_lines(filename);

    setup_BIC(sample, depth, prob_root, bic_root); // pre calculations
    
    // champions set calculation
    Champion_item champion_bics = champion_set(bic_root, Max_c(prob_root), Eps(prob_root)); 

    if (print_champs){
	ITERA(Champion_item, champion_item, champion_bics, next) {
	    printf("c=%f tree=[ ", champion_item->tau->c);
	    print_Tau(champion_item->tau);
	    printf("]\n");
	}
    }
    
    if(!renewalstr)
	renewalstr = most_frequent_word(prob_root);
    
    // starting the random number generator
    srand( seed ? seed : (unsigned int)time(NULL));
	
    Resamples resamples = resample(sample, prob_root, renewalstr,
				   size_resample_small, size_resample_large, number_resample_small);
	

    free(renewalstr);
    free_lines(sample);

    printf("\nBootstrap starting\n");
    bootstrap(resamples, champion_bics, depth);
}
