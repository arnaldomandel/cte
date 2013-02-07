/**
 * Context Tree Estimation
 * Selects the variable length Markov chain (VLMC) using the smallest maximizer criterion (SMC).
 */

#include "messages.h"
#include "read_file.h"


#include <stdio.h>

/*
 * Main method.
 * Read arguments, alphabet and samples.
 * Starts the BIC calculator.
 * Runs the champion trees selector.
 * Then runs the bootstrap method to return the selected Context Tree.
 */
int main(int argc, char** args) {
  char** lines = read_lines(args[1]);
  for (int i = 0; lines[i] != NULL; i++) {
    printf("%s\n", lines[i]);
  }
  free_lines(lines);
}
