#include <stdlib.h>
#include <string.h>
#include "read_line.h"

char *read_line(FILE *source) {
	size_t size_incr = 10;
	char *buf = (char*)malloc(sizeof(char) * size_incr);
	size_t buf_size = size_incr;
	size_t char_index = 0;
	char ch = 0;

	while ((ch = fgetc(source)) != '\n') {
		// resize check
		if (char_index == buf_size) {
			buf = (char*)realloc(buf, buf_size += size_incr);
		}
		buf[char_index++] = ch;
	}
	// resize check
	if (char_index == buf_size) {
		buf = (char*)realloc(buf, buf_size += size_incr);
	}
	buf[char_index] = '\0';
	return buf;
}

void free_line(char **line) {
	free(*line);
	*line = NULL;
}
