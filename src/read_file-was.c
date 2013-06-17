/*
 * Implementation of the read_lines method.
 */
/* Time-stamp: <2013/05/07 16:20:27 benavuya.ime.usp.br [benavuya] am> */

#include "glob.h"

#define BUFFER_SIZE 1024*64

/*
 * Reads one line from the specified line.
 * returns the line read, without the \n. Returns an empty string ('\0') in case of 2 consecutive '\n'
 */
char* read_line(FILE* file) {
    // we alocate an empty line, the 1 character is the string finalizer
    MEM(char*, line, (char*) malloc(1 * sizeof(char)));
  
    line[0] = '\0';
    int size_of_line = 1;
    char buffer[BUFFER_SIZE];

    int is_line_over = 0;

    do {
	int read_chars = 0;
	fgets(buffer, BUFFER_SIZE, file);
	read_chars = strlen(buffer);

	// we stop if we find a \n, of the end of the file. Also, if we find the \n, we ignore it
	if (buffer[read_chars-1] == '\n') {
	    is_line_over = 1;
	    read_chars--;
	    buffer[read_chars] = '\0';
	} else if (feof(file)) {
	    is_line_over = 1;
	}


	// realloc line to have its size plus the newly read characters
	MEM(,line, realloc(line, (size_of_line + read_chars)*sizeof(char)));
	strcat(line, buffer);
	size_of_line += read_chars;

    } while(!is_line_over);

    return line;
}


/*
 * Reads lines from a file, and return its lines in an array.
 * The last element of the array will point to NULL.
 */
char** read_lines(char* filename) {
    MEM(char**, lines, (char**) malloc(1*sizeof(char*)));
  
    lines[0] = NULL;
    int num_lines = 0;
    FILE* file;
    
    OPEN(file, filename, "r");

    while (!feof(file)) {
	//reads the new line
	char* new_line = read_line(file);
	// ignores empty lines
	if (strlen(new_line) == 0) {
	    free(new_line);
	    continue;
	}

	// extends the number of lines, adding 1 more
	num_lines++;
	MEM(,lines, (char**) realloc(lines, (num_lines+1) * sizeof(char*))); // the num_lines +1 is for the last, null line
	lines[num_lines] = NULL;

	// and add the new line to the array
	lines[num_lines - 1] = new_line;
    }

    fclose(file);
    return lines;
}

/*
 * Frees the memory allocated for the given lines.
 */
void free_lines(char** lines) {
    for (int i = 0; lines[i] != NULL; i++) {
	free(lines[i]);
    }
    free(lines);
    lines = NULL;
}
