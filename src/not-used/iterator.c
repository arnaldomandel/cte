#include "tree.h"
Tree_node next_node_up(Tree_node node, int depth);

Tree_node next_node_depth(Tree_node node, int depth) 
{
    Tree_node t;
    
    if (node->depth < depth && node->child)
	return next_node_depth(node->child, depth);
    return next_node_up(node, depth);
}

Tree_node next_node_up(Tree_node node, int depth)
{
    if (!node) return NULL;  // root node
    return  node->sibling ?
	next_node_depth(node->sibling, depth) :
	next_node_up(node->parent, depth);
}
