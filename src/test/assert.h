/**
 * Methods to facilitate testing.
 */

/**
 * Uses the == operator to verify if the 2 first arguments are the same.
 * Prints the error message (3rd argument) and exits if they aren't the same.
 */
void assert_equals(void* arg1, void* arg2, char* error_message);

/**
 * Uses the == operator to verify if the 2 first arguments are equals.
 * Prints the error message (3rd argument) and exits if they aren't the same.
 */
void assert_equals_int(int expected, int actual, char* error_message);
