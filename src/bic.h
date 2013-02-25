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

#endif /* BIC_H_ */
