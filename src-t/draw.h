enum tex_tree_type {
    SYNT,
    Q
};

    

Tree_node tau_to_tree(Tau tau);

char *synttree_string(Tree_node t);
char *qtree_string(Tree_node t);

void print_as_string(Tau tau, char type);

void print_as_tree(Tau tau, FILE *file, char type, int newpage);
