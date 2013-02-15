/*
 * Interface for the module that calculates the BIC trees.
 */

/*
 * Tao is the structure given as the return tree.
 * It was called Tao not to be mixed up with the tree data structure.
 */
typedef struct tao Tao;
struct tao {
  char* word;
  double probability;
};

/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char* alphabet, char** samples, int depth);

/**
 * Calculates the champion Tree
 */
Tao* calculate_BIC(double c);
