#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>
#include <stddef.h>

int read_from_descriptor(int fd, char **data, unsigned long *data_size);
int read_from_file(const char *path, char **data, size_t *data_size);

int write_to_file(const char *path, const char *data, unsigned long size);

void print_hex(const void *data, unsigned size);
void print_error(const char *str);

#endif