/*
 * Implementation of the tree methods.
 */

#include "tree.h"
#include "list.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// declaration of a function that is defined below
Tree_node create_child_node(Tree_node parent, char symbol, int type);


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
    struct tree_node dummy_node = { NULL, NULL, -1, 0, NULL, NULL, NULL };
    
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
  child->symbol = symbol;
  child->parent = parent;
  child->depth = parent ? parent->depth + 1 : 0;

  // allocate memory for the data structure corresponding to the given type.
  if (type == BIC) {
      child->bic_data = (Bic_data) calloc(1, sizeof(struct bic_data));
  } else if (type == PROB){
      child->prob_data = (Prob_data) calloc(1, sizeof(struct prob_data));
  }

  return child;
}

/* 
 * Creates an empty tree
 */ 
Tree_node Tree_create(int type) 
{
    return create_child_node(NULL, 0, type);
}


/*
 * Frees the memory used by a node, its children and siblings.
 */
void free_node(Tree_node node) {
  if (node->bic_data) {
    free(node->bic_data);
  }
  if (node->prob_data) {
    free(node->prob_data);
  }
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
    return (Tree_node) calloc(1, sizeof(struct tree_node));
}

/* 
 * Tree_node new_Tree_node() {
 *   Tree_node node = (Tree_node) malloc(sizeof(struct tree_node));
 *   node->child = NULL;
 *   node->sibling = NULL;
 *   node->parent = NULL;
 *   node->prob_data = NULL;
 *   node->bic_data = NULL;
 *   return node;
 * }
 */

void print_tree(Tree_node tree, char* given_label, int depth) 
{
    char lab[100], *l, *label;
    
    if (!tree) return;
    label = given_label;
    for(l = lab; *label; l++, label++)
	*l = *label;
    *l++ = tree->symbol;
    *l = 0;
    
    printf("%*s |%c| %9p %9p %9p ", depth, lab, tree->symbol, tree, tree->parent, tree->sibling);
    if(tree->prob_data)
	printf(" oc=%d prob=%f df=%d L=%f T=%f", tree->prob_data->occurrences,
	       tree->prob_data->probability, tree->prob_data->degrees_freedom, tree->prob_data->ell, tree->prob_data->T);
    else
	printf(" v=%f %s", tree->bic_data->v, tree->bic_data->critical ? "crit" : "");
    printf("\n");
    
    print_tree(tree->child, lab, depth);
    print_tree(tree->sibling, given_label, depth);
}

int outdegree(Tree_node root) 
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

/* 
 * Get the word label of a node
 */
char* word_of_node(Tree_node node) {
    char* word = (char*) calloc(node->depth+1, sizeof(char));
    for  ( ; node->depth; node = node->parent) 
	word[node->depth - 1] = node->symbol;
    return word;
}
