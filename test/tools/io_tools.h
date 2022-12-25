#ifndef IO_TOOLS
#define IO_TOOLS

#include <stdint.h>

#define TERM_RED "\x1B[31m"
#define TERM_GRN "\x1B[32m"
#define TERM_YEL "\x1B[33m"
#define TERM_BLU "\x1B[34m"
#define TERM_MAG "\x1B[35m"
#define TERM_CYN "\x1B[36m"
#define TERM_WHT "\x1B[37m"
#define TERM_RESET "\x1B[0m"

void print_hex(uint8_t *data, unsigned size);
const char *hex_to_str(uint8_t *data, unsigned size);

#endif
