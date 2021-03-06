/*
 * Implementation of the tree methods.
 */

#include "tree.h"
#include "list.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// declaration of a function that is defined below
Tree_node create_child_node(Tree_node parent, char symbol, int type);

void rec_print_tree(Tree_node tree, char* given_label, int depth);


/*
 * Searches for a node with the given symbol among the child of the given node.
 */
/*
Tree_node* get_create_child_node(Tree_node* parent, char symbol, int type) {
  // walk through every child node, until we find a corresponding symbol, or stop at insertion point

    Tree_node** prev = &(parent->child);

  printf(" %c", symbol);
  
  while(*prev && (*prev)->symbol < symbol)
      prev = &((*prev)->sibling);
  if(*prev && (*prev)->symbol == symbol)
      return *prev;
  
  // the correct node was not found, create a new one and put it at the correct position
  Tree_node* new_node = create_child_node(parent, symbol, type);
  new_node->sibling = *prev;
  *prev = new_node;

  printf("*");
  
  return new_node;
}
*/


Tree_node get_create_child_node(Tree_node parent, char symbol, int type) {
    Tree_node prev, cur;
    struct tree_node dummy_node;
    
    prev = &dummy_node;
    cur = prev->sibling = parent->child;
    while (cur && cur->symbol < symbol) {
	prev = cur;
	cur = cur->sibling;
    }
    
    if(cur && cur->symbol == symbol) {
	return cur;
    }
    
    // the correct node was not found, create a new one and put it at the correct position
    Tree_node new_node = create_child_node(parent, symbol, type);
    new_node->sibling = cur;
    if (prev == &dummy_node)
	parent->child = new_node;
    else
	prev->sibling = new_node;
    return new_node;
}


    

/*
 * Creates a node and associates the given node as its parent and the given symbol as its own.
 */
Tree_node create_child_node(Tree_node parent, char symbol, int type) {
  Tree_node child = new_Tree_node();
  child->type = type;
  child->symbol = symbol;
  child->parent = parent;
  child->depth = parent ? parent->depth + 1 : 0;

  /* 
   * // allocate memory for the data structure corresponding to the given type.
   * if (type == BIC) {
   *     MEM(,child->b, (Bic_data) calloc(1, sizeof(struct bic_data)));
   * } else if (type == PROB){
   *     MEM(,child->p, (Prob_data) calloc(1, sizeof(struct prob_data)));
   * }
   */
  
  return child;
}

/* 
 * Creates an empty tree
 */ 
Tree_node Tree_create(int type) 
{
    return create_child_node(NULL, ' ', type);
}


/*
 * Frees the memory used by a node, its children and siblings.
 */
void free_node(Tree_node node) {
  /* 
   * if (node->b) {
   *   free(node->b);
   * }
   * if (node->p) {
   *   free(node->p);
   * }
   */
  if (node->child) {
    free_node(node->child);
  }
  if (node->sibling) {
    free_node(node->sibling);
  }
  free(node);
}

/*
 * Searches for a node with the given symbol among the children of the given node.
 */
Tree_node get_child_node(Tree_node parent, char symbol) {
  if (!parent) 
    return NULL;
  
  // walk through every child node, until we find a corresponding symbol, or stop at last node
  // keep symbols ordered, so traversal is lexicographic
  Tree_node current_node = parent->child;
  while(current_node && current_node->symbol < symbol) 
    current_node = current_node->sibling;

  return current_node && current_node->symbol == symbol ? current_node : NULL;
}

/*
 * Instantiates a new Tree_node and set its default values
 */
Tree_node new_Tree_node() {
    MEM(Tree_node, t, (Tree_node) calloc(1, sizeof(struct tree_node)));
    return t;
}

/* 
 * Tree_node new_Tree_node() {
 *   Tree_node node = (Tree_node) malloc(sizeof(struct tree_node));
 *   node->child = NULL;
 *   node->sibling = NULL;
 *   node->parent = NULL;
 *   node->p = NULL;
 *   node->b = NULL;
 *   return node;
 * }
 */

void print_node(Tree_node node, char* label, int depth) 
{
    printf("%*s |%c| %9p %9p %9p %d", depth, label, node->symbol, node, node->parent, node->sibling, node->depth);
    if(node->type == PROB)
	printf(" oc=%d prob=%f df=%d L=%f T=%f", node->p.occurrences,
	       node->p.probability, node->p.df, node->p.ell, node->p.T);
    else
	printf(" v=%f %p %f %d %s", node->b.v, node->b.mate, node->b.lsum, node->b.dfsum, node->b.critical ? "crit" : "");
    printf("\n");
}


void print_tree(Tree_node tree, int depth) 
{
    rec_print_tree(tree, "", depth);
}

void rec_print_tree(Tree_node tree, char* given_label, int depth) 
{
    char lab[100], *l, *label;
    
    if (!tree) return;
    label = given_label;
    for(l = lab; *label; l++, label++)
	*l = *label;
    *l++ = tree->symbol;
    *l = 0;
    
    print_node(tree, lab, depth);
    rec_print_tree(tree->child, lab, depth);
    rec_print_tree(tree->sibling, given_label, depth);
}

int degree(Tree_node root) 
{
    int s = 0;
    ITERA(Tree_node, node, root->child, sibling)
	s++;
    return s;
}

/* 
 * Number of nodes in the tree
 */
int tree_size(Tree_node root) 
{
    if ( !root )
	return 0;
    int siz = 1;
    ITERA(Tree_node, node, root->child, sibling)
	siz += tree_size(node);
    return siz;
}

/*
 * Calculates the node depth.
 * If the given node is NULL, will return -1.
 * Return 0 for the root node.
 */
/* 
 * int node_depth(Tree_node node) {
 *   int depth = -1;
 *   //for (Tree_node* current_node = node; current_node != NULL; current_node = current_node->parent)
 *   ITERA(Tree_node, current_node, node, parent)
 *       depth++;
 *   return depth;
 * }
 */

/* Deprecated, keep depth as a node property */
int node_depth(Tree_node node) {
    return node->depth;
}



/* 
 * Iterator for nodes at a given depth.  Returns NULL at the end.
 */

Tree_node next_node_depth(Tree_node node, int depth)
{
    static int exported = 0;
    
    while (node->depth < depth && node->child)
	return next_node_depth(node->child, depth);
    if (!exported) {
	exported = 1;
	return node;
    }
    exported = 0;

    while(!node->sibling) {
	node = node->parent;
	if (!node)
	    return NULL;
    }
    
    return next_node_depth(node->sibling, depth);
}

/* 
 * Go down a digital tree
 */
Tree_node node_of_word(Tree_node tree, char *w) // w must be 0 terminated
{
    for(;*w;w++)
	tree = get_child_node(tree, *w);
    return tree;
}

Tree_node node_of_suffix(Tree_node tree, char *w) // w must be 0 terminated
{
    for(int i = strlen(w); i; i--)
	tree = get_child_node(tree, w[i - 1]);
    return tree;
}

/* 
 * Get the word label of a node
 */
char* word_of_node(Tree_node node) {
    MEM(char*, word, (char*) calloc(node->depth+1, sizeof(char)));
    
    for  ( ; node->depth; node = node->parent) 
	word[node->depth - 1] = node->symbol;
    return word;
}


Bivec new_bivec(int small, int large) 
{
    MEM(Bivec, vec, (Bivec) malloc(sizeof(struct bivec)));
    MEM(,vec->small, (double *) calloc(small, sizeof(double)));
    MEM(,vec->large, (double *) calloc(large, sizeof(double)));
    return vec;
}

void free_bivec(Bivec v)
{
    free(v->small);
    free(v->large);
    free(v);
}
