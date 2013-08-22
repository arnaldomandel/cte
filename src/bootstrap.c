/*
 * bootstrap.c
 */
/* Time-stamp: <2013/08/21 14:21:12 benavuya.ime.usp.br [benavuya] am> */
#include "glob.h"
#include "bic.h"
#include "tau.h"
#include "tree.h"
#include "champion_set.h"
#include "resample.h"
#include "bootstrap.h"
#include "stats.h"
    
// definition of functions implemented below
int accept_tree(Resamples resample, Tau current_tree, Tau next_tree);
void delta_tau(Resamples res, Tau current_tree, Tau next_tree, datapoint **deltatau, datapoint **ddeltatau);
// double threshold(int size);
void get_pre_trees(Resamples res, int depth) ;
void append_L_data(Tree_node node, Resamples res);
siz_by_samp L_tau_mat(Tau tau, Resamples res);
siz_by_samp matrix(int lines, int cols);
void freemat(siz_by_samp matrix, int lines);


// from student.c
void student(double *X, int Nx, double *Y, int Ny, double *both, double *left, double *right);

/*
 * Main method of the Bootstrap procedure.
 * Returns the tree that passes the t-student test
 */
Tau bootstrap(Resamples resample, Champion_item champion_set, int depth) {
    int number_of_trees = 0;
    Tau* champion_trees;
    Champion_item item;
    FILE *bstrap, *reg;
    Filename fbstrap, freg;
    //double pvalue;
    double beta, dev, tscore;
    datapoint *dt, *ddt;
    
    // complete the resample info
    get_pre_trees(resample, depth);
    mesg("Got pre trees.");
    
    append_L_data(resample->orig_tree, resample);
    mesg("Appended L data.");

    sprintf(fbstrap, "%s.bst", jobname);
    bstrap = fopen(fbstrap, "w");
    fprintf(bstrap,
	    "# bst\n# file: %s\n# depth: %d\n# split: %s\n# resamples: %d\n# sizes: %d\n# seed: %d\n\n",
	    real_filename, depth, renewalstr, number_resamples, num_sizes, seed);
    
    sprintf(freg, "%s.reg", jobname);
    reg = fopen(freg, "w");
    fprintf(reg, 
	    "# reg\n# file: %s\n# depth: %d\n# split: %s\n# resamples: %d\n# sizes: %d\n# seed: %d\n\n",
	    real_filename, depth, renewalstr, number_resamples, num_sizes, seed);
    
    // extract the Taus from the champion set to an array
    ITERA(, item, champion_set, next)
      number_of_trees++;
    MEM(,champion_trees, (Tau*) malloc(number_of_trees * sizeof(Tau)));
    item = champion_set;
    // for (int i = number_of_trees - 1; i >= 0; i--) {
    for (int i = 0; i < number_of_trees; i++) {
	champion_trees[i] = item->tau;
	item = item->next;
    }
    
    MEM(datapoint *, td, (datapoint *) calloc(resample->num_sizes, sizeof(datapoint)));
    MEMTZ(double, ddeltas, resample->num_sizes);

    // n = resample->num_sizes-1,  degrees of freedom = n-2
    tscore = tvalue(confid, resample->num_sizes - 3);
    for (int i = 0; i < number_of_trees - 1; i++) {
	//pprint_Tau(champion_trees[i]);
	//pprint_Tau(champion_trees[i+1]);
	DEB("tree %d\n", i);
	delta_tau(resample, champion_trees[i+1], champion_trees[i], &dt, &ddt);
	fprintf(bstrap, "%12.6f %12.6f %12.6f     ?\n", dt[0].val, dt[0].vlow, dt[0].vhigh);
	// fprintf(bstrap, "%12.6f %10.6f     ?        %12.6f\n", td[0].val, td[0].dev,
	//       td[0].val/threshold(resample->s[0].size));
	for(int j = 1; j < resample->num_sizes; j++) {
	    // fprintf(bstrap, "%12.6f %10.6f %12.6f %12.6f\n",
	    fprintf(bstrap, "%12.6f %12.6f %12.6f %12.6f %12.6f %12.6f\n",
		    dt[j].val, dt[j].vlow, dt[j].vhigh,
		    ddt[j].val, ddt[j].vlow, ddt[j].vhigh);
	    //ddeltas[j] = td[j].val; // GÂMBIA!!!!
		    // td[j].val/threshold(resample->s[j].size));
	    ddeltas[j] = ddt[j-1].val;
	    //ddeltas[j] = (dt[j].val - dt[0].val)/j;
	}
	fprintf(bstrap, "\n\n");
	//fprintf(reg, "%5d %13.10f\n", i, regress(ddeltas, resample->num_sizes));
	//full_regress(ddeltas, resample->num_sizes, &beta, &pvalue);
	//full_regress(ddeltas, resample->num_sizes, &beta, &pvalue, &tscore);
	// regress wants start at index 1
	full_regress(ddeltas, resample->num_sizes-1, &beta, &dev);
	fprintf(reg, "%5d %13.10f %13.10f %13.10f %13.10f\n", i, beta, dev*tscore, dev,
		p_value(beta/dev,resample->num_sizes - 3) );
    }
    fclose(bstrap);
    fclose(reg);
    
    free(champion_trees);
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
		mat[j][i] += node->p.L[j][i];
    }
    return mat;
}



//  Deprecated
double threshold(int size)
{
    return pow(size, 0.9);
}
