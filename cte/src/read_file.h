/*
 * This file describes the methods which can be used to read files.
 */
#ifndef READ_FILE_H_
#define READ_FILE_H_



/*
 * Read all lines from a file with given name.
 */
char** read_lines(char* filename);

/*
 * Frees the memory used by the lines.
 */
void free_lines(char** lines);

#endif /* READ_FILE_H_ */
