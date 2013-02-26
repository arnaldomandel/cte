/*
 * Definition of the Knuth-Morris-Pratt algorithm for text search.
 *
 *  Created on: Feb 26, 2013
 *      Author: onuki
 */

#ifndef KMP_H_
#define KMP_H_

/*
 * Returns the index of the first occurence of the match string in the source
 * string that happens after the init index.
 * Returns -1 if no match is found.
 */
int kmp(char* source, int init, char* match);


#endif /* KMP_H_ */
