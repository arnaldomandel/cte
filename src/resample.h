/*
 * resample.h
 *
 *  Created on: Feb 26, 2013
 *      Author: onuki
 */

#ifndef RESAMPLE_H_
#define RESAMPLE_H_

/*
 * Creates the resamples to be used on the bootstrapping
 */
char** resample(char** samples, char* most_frequent_word, int size_resample1, int size_resample2, int number_of_resamples);

#endif /* RESAMPLE_H_ */
