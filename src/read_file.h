/*
 * Methods which can be used to read files.
 */
/* Time-stamp: <2013/05/05 18:37:12 hutzpa [hutzpa] am> */
#ifndef READ_FILE_H_
#define READ_FILE_H_



/*
 * Read all lines from a file with given name.
 */
char** read_lines(char* filename);

/*
 * Read one line from an open file
 */
char* read_line(FILE* file);

/*
 * Frees the memory used by the lines.
 */
void free_lines(char** lines);

#endif /* READ_FILE_H_ */
