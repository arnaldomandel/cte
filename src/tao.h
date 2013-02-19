/*
 * Tao is the structure given as the return tree.
 * It was called Tao not to be mixed up with the tree data structure.
 */
typedef struct tao Tao;
typedef struct tao_item Tao_item;

struct tao {
  Tao_item* item;
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
