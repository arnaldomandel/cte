/*
 * Test for the kmp module
 */

#include <stdio.h>

#include "assert.h"
#include "kmp.h"
// declaration of a function on kmp.c that needs to be tested
int* kmp_table(char* w);


int main(int argc, char** argv) {
  int* t1 = kmp_table("ABCDABD");
  assert_equals_int(-1, t1[0], "Error on kmp_table\n");
  assert_equals_int(0, t1[1], "Error on kmp_table\n");
  assert_equals_int(0, t1[2], "Error on kmp_table\n");
  assert_equals_int(0, t1[3], "Error on kmp_table\n");
  assert_equals_int(0, t1[4], "Error on kmp_table\n");
  assert_equals_int(1, t1[5], "Error on kmp_table\n");
  assert_equals_int(2, t1[6], "Error on kmp_table\n");

  int* t2 = kmp_table("PARTICIPATE IN PARACHUTE");
  assert_equals_int(-1, t2[0], "Error on kmp_table\n");
  assert_equals_int(0, t2[1], "Error on kmp_table\n");
  assert_equals_int(0, t2[2], "Error on kmp_table\n");
  assert_equals_int(0, t2[3], "Error on kmp_table\n");
  assert_equals_int(0, t2[4], "Error on kmp_table\n");
  assert_equals_int(0, t2[5], "Error on kmp_table\n");
  assert_equals_int(0, t2[6], "Error on kmp_table\n");
  assert_equals_int(0, t2[7], "Error on kmp_table\n");
  assert_equals_int(1, t2[8], "Error on kmp_table\n");
  assert_equals_int(2, t2[9], "Error on kmp_table\n");
  assert_equals_int(0, t2[10], "Error on kmp_table\n");
  assert_equals_int(0, t2[11], "Error on kmp_table\n");
  assert_equals_int(0, t2[12], "Error on kmp_table\n");
  assert_equals_int(0, t2[13], "Error on kmp_table\n");
  assert_equals_int(0, t2[14], "Error on kmp_table\n");
  assert_equals_int(0, t2[15], "Error on kmp_table\n");
  assert_equals_int(1, t2[16], "Error on kmp_table\n");
  assert_equals_int(2, t2[17], "Error on kmp_table\n");
  assert_equals_int(3, t2[18], "Error on kmp_table\n");
  assert_equals_int(0, t2[19], "Error on kmp_table\n");
  assert_equals_int(0, t2[20], "Error on kmp_table\n");
  assert_equals_int(0, t2[21], "Error on kmp_table\n");
  assert_equals_int(0, t2[22], "Error on kmp_table\n");
  assert_equals_int(0, t2[23], "Error on kmp_table\n");


  assert_equals_int(0, kmp("aaaaaaa", 0, "a"), "Error on KMP\n");
  assert_equals_int(1, kmp("aaaaaaa", 1, "a"), "Error on KMP\n");
  assert_equals_int(0, kmp("abdeabd", 0, "abd"), "Error on KMP\n");
  assert_equals_int(4, kmp("abdeabd", 1, "abd"), "Error on KMP\n");
  assert_equals_int(4, kmp("qwerasdfzxcv", 0, "asdf"), "Error on KMP\n");

  printf("Tests finished successfully\n");
  return 0;
}
