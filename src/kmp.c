/*
 * Implementation of the Knuth-Morris-Pratt algorithm for text searching
 *
 * This implementation is an almost reproduction of the pseudo code existing in wikipedia.
 *
 *  Created on: Feb 26, 2013
 *      Author: onuki
 */

#include <string.h>
#include <stdlib.h>

/*
 * returns the kmp table, used on the kmp algorithm to know where to return when comparison fails.
 */
int* kmp_table(char* w) {
  int length = strlen(w);
  int* t = (int*) malloc((length + 1) * sizeof(int));
  int pos = 2; //the current position we are computing in T
  int cnd = 0; //the zero-based index in W of the next character of the current candidate substring

  // the first few values are fixed
  t[0] = -1;
  t[1] = 0;

  while (pos < length) {
    if (w[pos - 1] == w[cnd]) { //the substring continues
      cnd++;
      t[pos] = cnd;
      pos++;
    } else if (cnd > 0) { //it doesn't, but we can fall back
      cnd = t[cnd];
    } else { //we have run out of candidates.  Note cnd = 0
      t[pos] = 0;
      pos++;
    }
  }

  return t;
}

/*
 * Returns the index of the first occurence of the match string in the source
 * string that happens after the init index.
 * Returns -1 if no match is found.
 */
int kmp(char* source, int init, char* match) {
  int source_length = strlen(source);
  int match_length = strlen(match);

  int m = init; //the beginning of the current match in S
  int i = 0; //the position of the current character in W
  int* t = kmp_table(match); //the table

  while (m + i < source_length) {
    if (match[i] == source[m + i]) {
      if (i == match_length - 1)
        return m;
      i++;
    } else {
      m = m + i - t[i];
      if (t[i] > -1) {
        i = t[i];
      } else {
        i = 0;
      }
    }
  }
  // if we reach here, we have searched all of S unsuccessfully
  return -1;
}
