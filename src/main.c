/*
 * Context Tree Estimation
 * Selects the variable length Markov chain (VLMC) using the smallest maximizer criterion (SMC).
 */
/* Time-stamp: <2014/01/21 23:02:34 hutzpa [hutzpa] am> */

#include "glob.h"
#include "read_file.h"
#include "bic.h"
#include "tau.h"
#include "draw.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"
#include "messages.h"
#include "params.h"
#include "stats.h"

#include <time.h>
#include <getopt.h>

extern int champ_method;
extern double logN, scale;
void make_jobname(char *jname, char *spec);
		
/*
 * Main method.
 * Read arguments and samples.
 * Starts the BIC calculator.
 * Runs the champion trees selector.
 * Then runs the bootstrap method to return the selected Context Tree.
 */
int main(int argc, char** argv) {
    /* Parameters and defaults */
    char *filename = NULL;
    char *c_method = NULL;
    char *res_method = "e";
    char *jn = "%f-d%d-%T";
    int kept = 20;
    // int print_champs = 0;
    FILE *qtree, *champs, *prob;
    Filename fqtree, fchamps, fprob;
    Resamples resamples;
    
    /* Process options */
    int c;
    while (1) {
	static struct option long_options[] =  {
	    {"file",  required_argument, 0, 'f'},
	    {"depth",  required_argument, 0, 'd'},
	    {"kept-trees",  required_argument, 0, 'K'},
	    {"number-samples",    required_argument, 0, 'n'},
	    {"number-sizes",    required_argument, 0, 's'},
	    {"renewal-string",    required_argument, 0, 'r'},
	    {"seed",    required_argument, 0, 'S'},
	    //{"champ-method",    required_argument, 0, 'c'},
	    {"jobname",    required_argument, 0, 'j'},
	    //{"res_method",    required_argument, 0, 'R'},
	    //{"print-champs", no_argument, 0, 'C'},
	    {0, 0, 0, 0}
	};
	int option_index = 0;
	c = getopt_long (argc, argv, "f:d:K:n:r:s:S:k:j:m:Ch?", long_options, &option_index);
	//c = getopt_long (argc, argv, "f:d:K:n:r:s:S:c:k:j:m:R:Ch?", long_options, &option_index);
     
	if ( c == -1)
	    break;
	switch ( c ) {
	    SOPT('f', "filename", filename);
	    IOPT('d', "depth", depth);
	    IOPT('K', "kept", kept);
	    IOPT('n', "number of resamples", number_resamples);
	    IOPT('s', "number of sizes", num_sizes);
	    IOPT('S', "seed", seed);
	    SOPT('r', "renewal string", renewalstr);
	    //SOPT('c', "champ method", c_method);
	    SOPT('j', "job name", jn);
	    //SOPT('R', "resample method", res_method);
	    // NOPT('C', print_champs);
	case 'h':
	case '?':
	    erropt = 1;
	}
    }
    if(erropt){
	usage();
	exit(0);
    }
    
    if (optind < argc && !filename)
	filename = argv[optind];

    if(!filename)
	fatal_error(MISSING_FILENAME);

    realpath(filename, real_filename);

    mess("file %s\ndepth %d\nnum %d\nnsiz %d\nseed %d\nren %s\n",
	filename,depth,number_resamples,num_sizes,seed,renewalstr);
    champ_method = !c_method || c_method[0] != 'o';

    // starting the random number generator - seed neded for jobname
    if(!seed) seed = (unsigned int)time(NULL);
    srand( seed );
    mess("Seed: %d.\n", seed);

    /* 
     * if(jn)
     * 	strcpy(jobname, jn);
     * else 
     * 	sprintf(jobname, "%s-d%d-n%d-s%d-r%s-S%d-%06u",strrchr(filename, '/')+1, depth, number_resamples,
     * 		num_sizes, renewalstr ? renewalstr : "", seed, ((unsigned int)time(NULL) - 1367682157) >> 6);
     */
    make_jobname(jobname, jn);
    mess("Job name: %s.\n", jobname);
    
    Tree_node pre_root = Tree_create(PRE);
    Tree_node suf_root = Tree_create(SUF);

    mesg("Reading file.");
    
    char** sample = read_lines(filename);

    mesg("Calculating trees.");
    
    setup_BIC(sample, depth, pre_root, suf_root); // pre calculations

    mess("Number of nodes... p: %d (max %d)  s: %d (max %d)\n",
	 tree_size(pre_root), max_possible_nodes(pre_root, depth+1),
	 tree_size(suf_root), max_possible_nodes(pre_root, depth));
    
    // Save probabilities
    sprintf(fprob, "%s.prob", jobname);
    OPEN(prob, fprob, "w");
    fprint_tree_probs(prob, pre_root, depth);
    fclose(prob);
    
    // champions set calculation
    mesg("Finding champions.");
    Champion_item champion_bics = champion_set(suf_root, Max_c(pre_root), Eps(pre_root)); 
    mesg("Found");
    
    sprintf(fqtree, "%s.qtree", jobname);
    sprintf(fchamps, "%s.champ", jobname);
    OPEN(qtree, fqtree, "w");
    OPEN(champs, fchamps, "w");

    int number_of_trees = 0;
    double L0 = champion_bics->tau->L;
    ITERA(Champion_item, champion_item, champion_bics, next) {
	fprintf(champs, "# %d [ ", ++number_of_trees);
	    // printf("c=%f tree=[ ", champion_item->tau->c);
	fprint_Tau(champs, champion_item->tau);
	fprintf(champs, "\n");
	fpprint_Tau(champs, champion_item->tau, L0);
	fprintf(champs, "]\n");

	/* 
         * if (!champion_item->tau->item) {
	 *     fprintf(qtree, "\\begin{center}\\textsc{Root tree}\\end{center}\\n");
	 * }
         */
	
	fprint_as_tree(qtree, champion_item->tau, Q, 1);
	if (number_of_trees == kept) {
	    free(champion_item->next);
	    champion_item->next = NULL;
	}
    }
    fclose(qtree);
    fclose(champs);
    
    // mess("Champion_trees created %d trees\n",number_of_trees );
    mess("Kept %d champion trees.\n",number_of_trees );


    if (number_resamples) {
	if(!renewalstr)
	    renewalstr = most_frequent_word(pre_root);
    
	//renewalstr = renewalstr ? strdup(renewalstr) : most_frequent_word(pre_root); // strdup, so can free

	mesg("Resampling.");

	switch (*res_method) {
	case 'e':
	    mesg("Using ext method.\n");
	    resamples = resample_ext(sample, pre_root, renewalstr,
				     num_sizes, number_resamples);
	    break;
	
	case 'r':
	    mesg("Using rand method.\n");
	    resamples = resample_rand(sample, pre_root, renewalstr,
				      num_sizes, number_resamples);
	    break;
	default: mesg("Wrong resampling method, so...");
	}
	free_lines(sample);
	mesg("\nBootstrap starting");
	bootstrap(resamples, champion_bics, depth);
    }
}

void make_jobname(char *jname, char *spec)
{
    for (int i = 0; i < PATH_MAX && *spec; spec++) {
	if(*spec != '%')
	    jname[i++] = *spec;
	else {
	    switch (*++spec) {
	    case 'f': strcat(jname+i, strrchr(real_filename, '/')+1);
		break;
	    case 'r': strcat(jname+i, renewalstr);
		break;
	    case 'd': sprintf(jname+i, "%d", depth);
		break;
	    case 'n': sprintf(jname+i, "%d", number_resamples);
		break;
	    case 's': sprintf(jname+i, "%d", num_sizes);
		break;
	    case 'S': sprintf(jname+i, "%d", seed);
		break;
	    case 'T': sprintf(jname+i, "%06d", TIMESTAMP);
	    }
	    i = strlen(jname);
	}
    }
}

    
		
