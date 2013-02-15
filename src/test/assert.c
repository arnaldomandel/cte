/*
 * Implementation of methods defined in assert.h.
 *
 * Mostly methods to facilitate testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#define FLOAT_ERROR 0.0001f

/**
 * Uses the == operator to verify if the 2 first arguments are the same.
 * Both the 1st and 2nd arguments should be pointers.
 * Prints the error message (3rd argument) and exits if they aren't the same.
 */
void assert_equals(void* arg1, void* arg2, char* error_message) {
  if (arg1 == arg2) {
    return;
  }
  fprintf(stderr, error_message);
  exit(1);
}

/**
 * Uses the == operator to verify if the 2 first arguments are equals.
 * Prints the error message (3rd argument) and exits if they aren't the same.
 */
void assert_equals_int(int expected, int actual, char* error_message) {
  if (expected == actual) {
    return;
  }
  fprintf(stderr, error_message);
  fprintf(stderr, "expected: %d\nactual: %d\n", expected, actual);
  exit(1);
}


void assert_near_float(float expected, float actual, char* error_message) {
  if ((expected - FLOAT_ERROR < actual) && (actual < expected + FLOAT_ERROR)) {
    return;
  }
  fprintf(stderr, error_message);
  fprintf(stderr, "expected: %f\nactual: %f\n", expected, actual);
  exit(1);
}
