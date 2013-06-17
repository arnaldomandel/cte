/* Time-stamp: <2013/05/07 18:04:39 benavuya.ime.usp.br [benavuya] am> */

enum tex_tree_type {
    SYNT,
    Q
};

    

Tree_node tau_to_tree(Tau tau);

char *synttree_string(Tree_node t);
char *qtree_string(Tree_node t);

void print_as_string(Tau tau, char type);

void fprint_as_tree(FILE *file, Tau tau, char type, int newpage);
