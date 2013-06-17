#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "tau.h"
#include "tree.h"
#include "draw.h"

Tree_node tau_to_tree(Tau tau) /* bic tree */
{
    Tree_node tree = Tree_create(BIC);

    ITERA(Tau_item, item, tau->item, next) {
	Tree_node node = tree;
	for(int i = strlen(item->string); i; i--)
	    node = get_create_child_node(node, item->string[i-1], BIC);
    }
    return tree;
}

char *synttree_string(Tree_node t)
{
    if(!t) return (char *) calloc(1, 1);  /* empty string that can be freed */

    char *sib = synttree_string(t->sibling);
    char *chi = synttree_string(t->child);
    MEM(char *, s, calloc(strlen(sib) + strlen(chi) + 4, 1));
    s[0] = '[';
    s[1] = t->symbol;
    strcat(s, chi);
    s[strlen(s)] = ']';
    strcat(s, sib);
    free(sib);
    free(chi);
    return s;
}

char *qtree_string(Tree_node t)
{
    if(!t) return (char *) calloc(1, 1);  /* empty string that can be freed */

    char *sib = qtree_string(t->sibling);
    char *chi = qtree_string(t->child);
    MEM(char *, s, calloc(strlen(sib) + strlen(chi) + 7, 1));
    if(t->child) {
	s[0] = '[';
	s[1] = '.';
	s[2] = t->symbol;
	s[3] = ' ';
	strcat(s, chi);
	strcat(s, " ]");
    }
    else {
	s[0] = t->symbol;
	s[1] = ' ';
    }
    strcat(s, sib);
    free(sib);
    free(chi);
    return s;
}

void print_as_string(Tau tau, char type)
{
    Tree_node t = tau_to_tree(tau);
    char *s;
    
    switch (type) {
    case SYNT: s = synttree_string(t);
	break;
    case Q: s = qtree_string(t);
	break;
    }
    printf("%s\n", s);
    free(s);
    free_node(t);
}

void print_as_tree(Tau tau, FILE *file, char type, int newpage)
{
    Tree_node t = tau_to_tree(tau);
    char *s;
    
    switch (type) {
    case SYNT: s = synttree_string(t);
	fprintf(file, "\\synttree\n%s\n%s\n", s, newpage ? "\\newpage" : "\\vspace{4ex}");
	break;
    case Q: s = qtree_string(t);
	fprintf(file, "\\Tree %s\n%s\n", s, newpage ? "\\newpage" : "\\vspace{4ex}");
	break;
    }
    
    free(s);
    free_node(t);
}

