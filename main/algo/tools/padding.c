#include "padding.h"

void pad_block(uint8_t *block, const unsigned int actual_size, const unsigned int desired_size)
{
	const uint8_t filler = desired_size - actual_size;

	for (unsigned i = actual_size; i < desired_size; i++)
		block[i] = filler;
}