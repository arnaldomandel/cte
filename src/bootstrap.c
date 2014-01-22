/*
 * bootstrap.c
 */
/* Time-stamp: <2013/12/20 14:30:56 benavuya.ime.usp.br [benavuya] am> */
#include "glob.h"
#include "bic.h"
#include "tau.h"
#include "tree.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"
#include "stats.h"
    
// definition of functions implemented below
//int accept_tree(Resamples resample, Tau current_tree, Tau next_tree);
//void delta_tau(Resamples res, Tau current_tree, Tau next_tree, datapoint **deltatau, datapoint **ddeltatau);
// double threshold(int size);
void get_pre_trees(Resamples res, int depth) ;
void append_L_data(Tree_node node, Resamples res);
siz_by_samp L_tau_mat(Tau tau, Resamples res);
siz_by_samp matrix(int lines, int cols);
void freemat(siz_by_samp matrix, int lines);


// from student.c
//void student(double *X, int Nx, double *Y, int Ny, double *both, double *left, double *right);

/*
 * Main method of the Bootstrap procedure.
 * Returns the tree that passes the t-student test
 */
Tau bootstrap(Resamples resample, Champion_item champion_set, int depth) {
    FILE *bstrap;
    Filename fbstrap;
    //double pvalue;
    
    // complete the resample info
    get_pre_trees(resample, depth);
    mesg("Got pre trees.");
    
    append_L_data(resample->orig_tree, resample);
    mesg("Appended L data.");

    sprintf(fbstrap, "%s.bst", jobname);
    bstrap = fopen(fbstrap, "w");
    fprintf(bstrap,
	    "# bst\n# file: %s\n# depth: %d\n# split: %s\n# resamples: %d\n# sizes: %d : ",
	    real_filename, depth, renewalstr, number_resamples, num_sizes);
    for(int j = 0; j < resample->num_sizes; j++)
	    fprintf(bstrap, "%d ", resample->s[j].size);
    fprintf(bstrap, "\n# seed: %d\n\n", seed);
    
    int tree_num = 0;
    ITERA(Champion_item, current_tree, champion_set, next) {
	tree_num++;
	DEB("tree %d\n", tree_num);
	fprintf(bstrap, "{%d}\n", tree_num);
	siz_by_samp L_tau = L_tau_mat(current_tree->tau, resample);
	
	for(int j = 0; j < resample->num_sizes; j++) {
	    fprintf(bstrap, "[%d] ", j);
	    for (int i = 0; i < resample->num_resamples; i++)
		fprintf(bstrap, "%12.8f", -L_tau[j][i]);
	    fprintf(bstrap, "\n");
	}
	fprintf(bstrap, "\n");
    }
    fclose(bstrap);
    
    return champion_set->tau;
}



/*
 * Calculates delta^(tau_i, tau_{i+1})(n,b)
 */
void delta_tau(Resamples res, Tau current_tree, Tau next_tree, datapoint **deltatau, datapoint **ddeltatau) {

    MEM(double *, delta_L, (double *) calloc(res->num_resamples, sizeof(double)));
    MEM(datapoint *, tau_dat, (datapoint *) calloc(res->num_sizes, sizeof(datapoint)));
    MEM(datapoint *, tau_ddat, (datapoint *) calloc(res->num_sizes, sizeof(datapoint)));
    
    siz_by_samp L_tau1 = L_tau_mat(current_tree, res);
    siz_by_samp L_tau2 = L_tau_mat(next_tree, res);

    for (int j = 0; j < res->num_sizes; j++) {
	memcpy(delta_L, L_tau1[j], res->num_resamples * sizeof(double));
	for (int i = 0; i < res->num_resamples; i++)
	    delta_L[i] -= L_tau2[j][i];
	tau_dat[j] = mid_and_errors(delta_L, res->num_resamples);
	for (int i = 0; i < res->num_resamples; i++)
	    delta_L[i] = delta_L[i+1] - delta_L[i];
	tau_ddat[j] = mid_and_errors(delta_L, res->num_resamples-1);
	//tau_dat[j] = av_and_dev(delta_L, res->num_resamples);
    }
    freemat(L_tau1, res->num_sizes);
    freemat(L_tau2, res->num_sizes);
    free(delta_L);
    *deltatau = tau_dat;
    *ddeltatau = tau_ddat;
}


/* 
 * Creates a pre tree for each sample
 */
void get_pre_trees(Resamples res, int depth) 
{
    char* sample[2];  // quirk of setup_BIC
    sample[1] = NULL;

    for (int j = 0; j < res->num_sizes; j++) {
	MEM(, res->s[j].tree, (Tree_node *) calloc(res->num_resamples, sizeof(Tree_node)));
	for (int i = 0; i < res->num_resamples; i++) {
	    mess("Analysing samples: %2d %3d\r", j, i);
	    res->s[j].tree[i] = Tree_create(PRE);
	    sample[0] = res->s[j].sample[i];
	    setup_BIC(sample, depth, res->s[j].tree[i], NULL);
	    //DEB("Tree level %2d [%4d]: %4d\n", j, i, tree_size(res->s[j].tree[i]));
	}
    }
    mesg("");
}

// returns a zero matrix
siz_by_samp matrix(int lines, int cols)
{
    MEM(double **, mat, (siz_by_samp) malloc(lines * sizeof(double *)));
    for(int i = 0; i < lines; i++) {
	MEM(, mat[i], (double *) calloc( cols, sizeof(double)));
    }
    
    return mat;
}

void freemat(siz_by_samp mat, int lines)
{
    for(int i = 0; i < lines; i++)
	free(mat[i]);
    free(mat);
}

/* 
 * 
 * Calculate Lw for every word in a pre tree and every sample.
 * Completes the pre tree.
 * 
 */
void append_L_data(Tree_node pre_node, Resamples res) 
{
    char *w = word_of_node(pre_node);

    pre_node->p.L = matrix(res->num_sizes, res->num_resamples);

    for(int j = 0; j < res->num_sizes; j++)
	for(int i = 0; i < res->num_resamples; i++)
	    pre_node->p.L[j][i] = Lw(res->s[j].tree[i], w);
    free(w);
    if(pre_node->child)
	append_L_data(pre_node->child, res);
    if(pre_node->sibling)
	append_L_data(pre_node->sibling, res);
}

/* 
 *  L_tau matrix, for a given champ tree tau; just the sum of corresponding matrices for words 
 */
siz_by_samp L_tau_mat(Tau tau, Resamples res)
{
    siz_by_samp mat = matrix(res->num_sizes, res->num_resamples);
    ITERA(Tau_item, item, tau->item, next) {
	Tree_node node = node_of_word(res->orig_tree, item->string);
	for(int j = 0; j < res->num_sizes; j++)
	    for(int i = 0; i < res->num_resamples; i++)
		mat[j][i] += node->p.L[j][i] / res->s[j].size; /* had forgotten to divide by size */
		/* mat[j][i] += node->p.L[j][i]; */
    }
    return mat;
}



//  Deprecated
double threshold(int size)
{
    return pow(size, 0.9);
}
