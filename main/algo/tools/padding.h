#ifndef PADDING
#define PADDING

#include <stdint.h>

void pad_block(uint8_t *block, const unsigned int actual_size, const unsigned int desired_size);

#endif