/*
 * Test for the bic calculator
 */
/* Time-stamp: <2013/05/07 18:06:52 benavuya.ime.usp.br [benavuya] am> */

#include "glob.h"
#include "params.h"
#include "read_file.h"
#include "bic.h"
#include "tree.h"
#include "champion_set.h"
#include "tau.h"
#include "draw.h"
#include <math.h>
#include <unistd.h>

extern double logN, scale;
extern int sample_size;
extern int champ_method;
/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
    char** samples;
    int depth;
    double max_c;// = strtod(argv[3], NULL);
    double eps;// = strtod(argv[4], NULL);
    int opt, k, i;
    long long int ki, num;
    // FILE  *critC, *critTrees;
    char arq[100];
    char *filename = NULL; 
    //Tau ces[100000];
    FILE *trees = NULL;
    char *draw_type = "";
    
    while((opt = getopt(argc, argv, "of:d:t:k:")) != -1) 
	switch ( opt ) {
	    SOPT('f', "filename", filename);
	    IOPT('d', "depth", depth);
	    SOPT('t', "drawing", draw_type);
	    FOPT('k', "scale", scale);
	case 'o': champ_method = 0;
	}
    if(erropt) {
	printf("Argument missing\n");
	exit(1);
    }
    
    samples = read_lines(filename);

    
    Tree_node pre_root = Tree_create(PRE);
    Tree_node suf_root = Tree_create(SUF);
    setup_BIC(samples, depth, pre_root, suf_root);

    k = degree(pre_root);
    ki = 1;
    num = sample_size;
    printf("num: %lld\n",num);
  
    for (i = 1; i <= depth+1 && ki <= num - i; i++)
	ki *= k;
    if (ki <= num -i) ki*=k;
    
    num = (ki-1)/(k-1)+(depth-i+2)*num;
    // printf("%lld %lld %d %lld\n",ki,(ki-1)/(k-1),i,num);
    // print_tree(pre_root, -(depth+1));
    printf("pre_root: %d nodes, estimated max nodes: %lld\n", tree_size(pre_root), num);
    //printf("\n-----------------\n");
  
    // print_tree(suf_root, (depth+1));
    // int n = size_of_sample();
    max_c = Max_c(pre_root);
    eps = Eps(pre_root);
    printf("Calculated max_c = %f, epsilon = %11.10f\n", max_c, eps);
  
    Champion_item champs = champion_set(suf_root, max_c, eps);
    
    DEBS("Champ set");
  
    int numtrees = 0;

    if(*draw_type) {
	for (char *p = filename; *p; p++)
	    if( *p == '_' ) *p = '-';
	sprintf(arq, "../results/%s-%d-%s-trees.tex", strrchr(filename, '/'), depth, draw_type);
	OPEN(trees, arq, "w");
    }
    
    i = 0;

    double delta_c = max_c;
    double last_c = -max_c;
    ITERA(Champion_item, cs, champs, next) {
	//fprintf(critC, "%f\n", cs->tau->c);
	//ces[i++] = cs->tau;
	//pprint_Tau(cs->tau);
	// print_as_string(cs->tau, Q);
	if(trees)
	    fprint_as_tree(trees, cs->tau, (*draw_type == 's' ? SYNT : Q), 1);
      
	if(cs->tau->c - last_c < delta_c)
	    delta_c = cs->tau->c - last_c;
	last_c = cs->tau->c;
	//printf("\n");
	numtrees++;
    }

    //double last = ces[i-1]->L;
    
    /* 
     * for(i--; i; i--) {
     *     //printf("%16f\n", ces[i-1]->L);
     *     pprint_Tau(ces[i-1]);
     *     /\* 
     *      * fprintf(critC, "%16f\n", (last - ces[i-1]->L)/logN);
     *      * last = ces[i-1]->L;
     *      *\/
     *     /\* 
     *      * if(argc > 3) {
     *      * 	  fprintf(critTrees, "%5d: [\n", numtrees - i);
     *      * 	  ITERA(Tau_item, item, ces[i-1]->item, next)
     *      * 	      fprintf(critTrees, "        %s\n", item->string);
     *      * 	  fprintf(critTrees, "       ]\n");
     *      * }
     *      *\/
     * }
     */
  
    printf("pre_root: %d nodes, estimated max nodes: %lld\n", tree_size(pre_root), num);
    printf("Number of champ trees: %d\n",numtrees);
    printf("Delta c: %16.15g\n", delta_c);
//    printf("Delta c: %16.15g       %g  %g %g\n", delta_c, 1/delta_c, logN, scale);
}
