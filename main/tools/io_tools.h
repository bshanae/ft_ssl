#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>

int read_from_descriptor(char **data, int fd);
int read_from_file(char **data, const char *path);

void print_hex(uint8_t *data, unsigned size);
void print_error(const char *str);

#endif