/*
 * Implementation of the tree methods.
 */

#include "tree.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// declaration of a function that is defined below
Tree_node* create_child_node(Tree_node* parent, char symbol, int type);


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

Tree_node dummy_node = { NULL, NULL, -1, NULL, NULL, NULL };

Tree_node* get_create_child_node(Tree_node* parent, char symbol, int type) {
    Tree_node *prev, *cur;
    
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
    Tree_node* new_node = create_child_node(parent, symbol, type);
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
Tree_node* create_child_node(Tree_node* parent, char symbol, int type) {
  Tree_node* child = new_Tree_node();
  child->symbol = symbol;
  child->parent = parent;

  // allocate memory for the data structure corresponding to the given type.
  if (type == BIC) {
      child->bic_data = (Bic_data*) calloc(1, sizeof(Bic_data));
  } else if (type == PROB){
      child->prob_data = (Prob_data*) calloc(1, sizeof(Prob_data));
  }

  return child;
}

/* 
 * Creates an empty tree
 */ 
Tree_node* Tree_create(int type) 
{
    return create_child_node(NULL, 0, type);
}


/*
 * Frees the memory used by a node, its childs and siblings.
 */
void free_node(Tree_node* node) {
  if (node->bic_data != NULL) {
    free(node->bic_data);
  }
  if (node->prob_data != NULL) {
    free(node->prob_data);
  }
  if (node->child != NULL) {
    free_node(node->child);
  }
  if (node->sibling != NULL) {
    free_node(node->sibling);
  }
  free(node);
}

/*
 * Searches for a node with the given symbol among the children of the given node.
 */
Tree_node* get_child_node(Tree_node* parent, char symbol) {
  if (parent == NULL) {
    return NULL;
  }
  // walk through every child node, until we find a corresponding symbol, or stop at last node
  // keep symbols ordered, so traversal is lexicographic
  Tree_node* current_node = parent->child;
  while(current_node != NULL && current_node->symbol < symbol) 
    current_node = current_node->sibling;

  return current_node && current_node->symbol == symbol ? current_node : NULL;
}

/*
 * Instantiates a new Tree_node and set its default values
 */
Tree_node* new_Tree_node() {
  Tree_node* node = (Tree_node*) malloc(sizeof(Tree_node));
  node->child = NULL;
  node->sibling = NULL;
  node->parent = NULL;
  node->prob_data = NULL;
  node->bic_data = NULL;
  return node;
}

void print_tree(Tree_node* tree, char* given_label) 
{
    char lab[100], *l, *label;
    
    if (!tree) return;
    label = given_label;
    for(l = lab; *label; l++, label++)
	*l = *label;
    *l++ = tree->symbol;
    *l = 0;
    
    printf("%s |%c| %p %p %p ", lab, tree->symbol, tree, tree->parent, tree->sibling);
    if(tree->prob_data)
	printf(" oc=%d prob=%f df=%d L=%f", tree->prob_data->occurrences,
	       tree->prob_data->probability, tree->prob_data->degrees_freedom, tree->prob_data->ell);
    else
	printf(" v=%f %s", tree->bic_data->v, tree->bic_data->critical ? "crit" : "");
    printf("\n");
    
    print_tree(tree->child, lab);
    print_tree(tree->sibling, given_label);
}
