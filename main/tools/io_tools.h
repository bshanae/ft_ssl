#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>

int read_from_descriptor(char **data, int fd);
int read_from_file(const char *path, char **data);

int write_to_file(const char *path, const char *data, const unsigned long size);

void print_hex(const void *data, unsigned size);
void print_error(const char *str);

#endif