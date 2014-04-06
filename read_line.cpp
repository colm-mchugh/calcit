#include <stdlib.h>
#include <string.h>
#include "read_line.h"

#define SIZE_INCREMENT 10

// Read a line of characters into a buffer from the given source.
// Terminate upon newline character, increase memory as needed in
// increments of SIZE_INCREMENT. TODO: upper limit on number of increments
char *read_line(FILE *source) {
	char *buf = (char*)malloc(sizeof(char) * SIZE_INCREMENT);
	size_t buf_size = SIZE_INCREMENT;
	size_t char_index = 0;
	char ch = 0;

	while ((ch = fgetc(source)) != '\n') {
		// resize check
		if (char_index == buf_size) {
			buf = (char*)realloc(buf, buf_size += SIZE_INCREMENT);
		}
		buf[char_index++] = ch;
	}
	// resize check
	if (char_index == buf_size) {
		buf = (char*)realloc(buf, buf_size += SIZE_INCREMENT);
	}
	buf[char_index] = '\0';
	return buf;
}

// Free a buffer previously allocated with read_line
void free_line(char **line) {
	free(*line);
	*line = NULL;
}
