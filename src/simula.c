#include "glob.h"
#include "tree.h"
#include "tau.h"
#include "read_file.h"
#include "params.h"
#include <math.h>
#include <time.h>

char next_char(char *buf, Tree_node tree, double *loglike);
Tree_node parse_prob(FILE *probfile);
Tau choose_tau_from_champ(FILE *champfile, int num, char *maxs);
void prune(Tree_node tree, Tau tau);
void usage();

#define BUFSIZE 10000

    


int main(int argc, char **argv)
{
    Filename fname;
    FILE *prob, *champ, *out;
    int  treenum = 0, size = 0, opt;
    char buf[100], *job = NULL;
    Tree_node tree;
    Tau tau;
    double logL = 0;
    int seed = 0;
    
    while((opt = getopt(argc, argv, "j:t:s:")) != -1) 
	switch ( opt ) {
	    SOPT('j', "jobname", job);
	    IOPT('t', "tree num", treenum);
	    IOPT('s', "size", size);
	    IOPT('S', "seed", seed);
	}
    if(erropt || !job || !treenum || !size) {
	usage();
	exit(1);
    }

    mess("job %s tree %d size %d\n", job, treenum, size);
        
    sprintf(fname, "%s.prob", job);
    OPEN(prob, fname, "r");
    //prob = fopen(fname, "r");
    //if(!prob) err(1, "%s",fname);
    tree = parse_prob(prob);
    fclose(prob);
    mess("Parsed %s\n", fname);
    
    sprintf(fname, "%s.champ", job);
    OPEN(champ, fname, "r");
    tau = choose_tau_from_champ(champ, treenum, buf);
    fclose(champ);
    mess("Chosen tree %d from %s; buffer: %s\n", treenum, fname, buf);
    
    prune(tree, tau);
    
    // starting the random number generator
    if(!seed) seed = (unsigned int)time(NULL);
    srand( seed );
    mess("Seed: %d.\n", seed);

    // Maybe initialize buf otherwise
    sprintf(fname, "%s-%d-%d-%d.sim", job, treenum, size, seed);
    OPEN(out, fname, "w");
    for(int i = 0; i < size; i++) {
	fputc(next_char(buf, tree, &logL), out);
	mess("%8d\r", i);
    }
    mesg("");
    fputc('\n', out);
    fclose(out);
    printf("Log likelihood: %lg\nEntropy per char: %lg\n", logL, -logL/size);
}



Tree_node node_of_max_suffix(Tree_node tree, char *w)
{
    Tree_node t = node_of_word(tree,w);
    
    while(! (t && t->p.occurrences) ) 
	t = node_of_word(tree, ++w);
    return t;
}

/* 
 * Next char in the Markov chain in which:
 *   buf is the state
 *   tree encodes the transitions - suffixes are in a normal digital tree
 */
char next_char(char *buf, Tree_node tree, double *loglike)
{
    double prob = rand() / (RAND_MAX+1.0);
    char *b;
    
    Tree_node t = node_of_max_suffix(tree, buf);
    // mess("buf: %6s  suf: %6s  prob: %f\n", buf, word_of_node(t), prob);
    
    if(!t)
	printf("porra de palavra: %s\n", buf);
    
    if(!t->child) {
	mesg("Stomped!");
	print_node(t,"",6);
	exit(1);
    }
    /* Choose the first symbol s such that the sum of probabilities of symbols <= s is <= prob */
    for(t = t->child; t->sibling && prob > t->p.probability; t = t->sibling)
	prob -= t->p.probability;
    *loglike += log(t->p.probability);
    for(b = buf + 1; *b; b++)
	b[-1] = *b;
    return b[-1] = t->symbol;
}

Tree_node get_create_node_of_suffix(Tree_node pre_tree, char *w) // w must be 0 terminated
{
    for(int i = strlen(w); i; i--)
	pre_tree = get_create_child_node(pre_tree, w[i - 1], PRE);
    return pre_tree;
}

Tree_node parse_prob(FILE *probfile) // file already open
{
    char line[BUFSIZE], *tok, c;
    Tree_node tree, t;
    double prob;
    
    while(fgets(line, BUFSIZE, probfile)){
	if(*line == '#')
	    continue;
	tok = strtok(line, " ");
	if(!tok) break;
	
	if(*tok == '-')
	    t = tree = Tree_create(PRE);
	else
	    t = node_of_word(tree, tok);
	
	while((tok = strtok(NULL, " -\n"))) {
	    sscanf(tok, "%c:%lf", &c, &prob);
	    Tree_node tc = get_create_child_node(t, c, PRE);
	    tc->p.probability = prob;
	}
    }
    return tree;
}

Tau choose_tau_from_champ(FILE *champfile, int num, char *maxs)
{
    char *line, *tok;
    int i, s;
    Tau t = new_Tau();

    for(i = 1; i < num; i++)
	read_line(champfile);
    line = read_line(champfile);
    tok = strtok(line, "# ");
    if(atoi(tok) != num) {
	mess("line %d: %s\n", num, line);
	exit(1);
    }
    int maxl = 0;
    while((tok = strtok(NULL, " []"))) {
	if((s = strlen(tok)) > maxl) {
	    strcpy(maxs, tok);
	    maxl = s;
	}
	insert_tau_item(t, tok, 1.);
    }
    return t;
}

// Mark all usable nodes
void prune(Tree_node tree, Tau tau)  
{
    ITERA(Tau_item, item, tau->item, next) 
	    node_of_word(tree, item->string)->p.occurrences = 1;
}
/* 
 * {
 *     ITERA(Tau_item, item, tau->item, next) 
 * 	ITERA(Tree_node, t, node_of_suffix(tree, item->string), parent)
 * 	    t->p.occurrences = 1;
 * }
 */

void usage() 
{
    printf("\nUsage: simula -j jobname -t num -s size [-S seed]\n\n\
       jobname refers to a cte jobname (needs .champ and .prob)\n\
       num     is the number of a champ tree from jobname.champ\n\
       size    is the length of the string to generate\n\
       seed    initializes rand()\n\
");
}
