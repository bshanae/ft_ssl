#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>

void print_hex(uint8_t *data, unsigned size);
const char *hex_to_str(uint8_t *data, unsigned size);

#endif
