/*
 * Tao is the structure given as the return tree.
 * It was called Tao not to be mixed up with the tree data structure.
 */
#ifndef TAO_H_
#define TAO_H_




typedef struct tao Tao;
typedef struct tao_item Tao_item;

struct tao {
  Tao_item* item;
  double c;
};

struct tao_item {
  char* string;
  double probability;
  Tao_item* next;
};

/*
 * Adds a item to the given tao.
 */
void insert_tao_item(Tao* tao, char* string, double prob);

/*
 * Returns a new Tao.
 */
Tao* new_Tao();


/*
 * Frees the memory allocated by this Tao.
 */
void free_Tao(Tao* t);


/*
 * Verifies if both taos are equals, ie, have the same words.
 */
int equals_Tao(Tao* t1, Tao* t2);


/*
 * Prints the Tao to the stdout.
 */
void print_Tao(Tao* tao);

#endif /* TAO_H_ */
