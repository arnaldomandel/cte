/*
 * Interface for the module that calculates the BIC trees.
 */
#ifndef BIC_H_
#define BIC_H_



#include "tau.h"
/*
 * Sets up the BIC calculator. Performs the initial calculations that are
 * independent of the C (cost) value.
 */
void setup_BIC(char** samples, int depth);

/**
 * Calculates the champion Tree given the cost.
 */
Tau* calculate_BIC(double c);

/*
 * Returns the most frequent word from the samples whose length is the max word size.
 */
char* most_frequent_word();

/*
 * Returns the size of the sample used to calculate the BIC.
 */
int size_of_sample();



/*
 * Calculates Ltau on the current Prob Tree
 */
double L_tau(Tau* tau);


#endif /* BIC_H_ */
