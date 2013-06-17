/*
 * Implementation of the digital tree methods.
 */
/* Time-stamp: <2013/06/17 15:41:42 hutzpa [hutzpa] am> */

#include "glob.h"
#include "tree.h"

// declaration of a function that is defined below
Tree_node create_child_node(Tree_node parent, char symbol, int type);

void rec_print_tree(Tree_node tree, char* given_label, int depth);
void rec_fprint_tree_probs(FILE *file, Tree_node tree, char* given_label, int depth);


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
  if (node->child) 
    free_node(node->child);
  if (node->sibling)
    free_node(node->sibling);
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
    // MEMPZ(Tree_node, t, 1);
    return t;
}

void print_node(Tree_node node, char* label, int depth) 
{
    printf("%*s |%c| %9p %9p %9p %9p %2d", depth, label, node->symbol, node,
	   node->parent, node->child, node->sibling, node->depth);
    if(node->type == PRE)
	printf(" oc=%d prob=%f df=%d L=%f", node->p.occurrences,
	       node->p.probability, node->p.df, node->p.ell);
    else
	printf(" v=%f %p %f %d %s", node->s.v, node->s.mate, node->s.lsum, node->s.dfsum, node->s.critical ? "crit" : "");
    printf("\n");
}


void print_tree(Tree_node tree, int depth) 
{
    printf("%-*s |c| node      parent    child     sibling    d", depth, "label");
    if(tree->type == SUF)
	printf("          mate      lsum     dfsum    ");
    printf("\n");
    
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
Tree_node node_of_word(Tree_node pre_tree, const char *w) // w must be 0 terminated
{
    for(;*w;w++)
	pre_tree = get_child_node(pre_tree, *w);
    return pre_tree;
}

Tree_node node_of_suffix(Tree_node suf_tree, const char *w) // w must be 0 terminated
{
    for(int i = strlen(w); i; i--)
	suf_tree = get_child_node(suf_tree, w[i - 1]);
    return suf_tree;
}

/* 
 * Tree_node node_of_max_suffix(Tree_node tree, char *w) // w must be 0 terminated
 * {
 *     Tree_node t;
 *     while (!(t = node_of_suffix(tree, w))) w++;
 *     mess("suffix: %s t: %p\n", w, t);
 *     
 *     return t;
 * }
 */

/* 
 * {
 *     Tree_node t = tree;
 *     for(int i = strlen(w); i && t; i--) {
 * 	tree = t;
 * 	t = get_child_node(t, w[i - 1]);
 * 	mess("tree: %p t: %p w[i-1]: %c\n", tree, t, w[i-1]);
 *     }
 *     
 *     return tree;
 * }
 */

/* 
 * Get the word label of a node
 */
char* word_of_node(Tree_node pre_node) {
    MEM(char*, word, (char*) calloc(pre_node->depth+1, sizeof(char)));
    // MEMTZ(char, word, pre_node->depth+1);
    for  ( ; pre_node->depth; pre_node = pre_node->parent) 
	word[pre_node->depth - 1] = pre_node->symbol;
    return word;
}

/*
 * Gets the suffix label of a node
 */
char* suffix_of_node(Tree_node suf_node) {
  int length = suf_node->depth;
  MEM(char*, suffix, (char*) malloc((length+1)* sizeof(char))); // +1 for the \0 terminator
  //MEMTZ(char, suffix, length+1);
  int i = 0;
  ITERA(Tree_node, current_node, suf_node, parent)
      suffix[i++] = current_node->symbol;
  suffix[length] = '\0';
  return suffix;
}

void fprint_node_probs(FILE *file, Tree_node pre_node, char* label, int depth) 
{
    fprintf(file, "%-*s -", depth+1, label+1);
    ITERA(Tree_node, t, pre_node->child, sibling)
	fprintf(file, " %c:%f", t->symbol, t->p.probability);
    fprintf(file, "\n");
}


void fprint_tree_probs(FILE *file, Tree_node pre_tree, int depth) 
{
    rec_fprint_tree_probs(file, pre_tree, "", depth);
}

void rec_fprint_tree_probs(FILE *file, Tree_node pre_tree, char* given_label, int depth) 
{
    if (!pre_tree) return;
    char lab[100], *l, *label;  // 100 is a max depth!
    
    label = given_label;
    for(l = lab; *label; l++, label++)
	*l = *label;
    *l++ = pre_tree->symbol;
    *l = 0;
    
    fprint_node_probs(file, pre_tree, lab, depth);
    rec_fprint_tree_probs(file, pre_tree->child, lab, depth);
    
    rec_fprint_tree_probs(file, pre_tree->sibling, given_label, depth);
}
