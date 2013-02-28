/*
 * Test for the bic calculater
 */
#include "read_file.h"
#include "bic.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Test the result for the bic calculator.
 */
int main(int argc, char** argv) {
  char** samples = read_lines(argv[1]);
  double c = strtod(argv[2], NULL);

  setup_BIC(samples, 5);

  Tau* tao = calculate_BIC(c);

  Tau_item* item = tao->item;
  while (item != NULL) {
    printf("%s ", item->string);
    item = item->next;
  }
  printf("\n");
}
