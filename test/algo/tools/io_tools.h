#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>

#define TERM_RED "\033[91m"
#define TERM_GREEN "\033[92m"
#define TERM_RESET "\x1B[0m"

void print_hex(uint8_t *data, unsigned size);
const char *hex_to_str(uint8_t *data, unsigned size);

#endif
