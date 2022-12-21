#include <printf.h>
#include "io_tools.h"

void print_hex(uint8_t *hash, unsigned size)
{
	for (unsigned int i = 0; i < size; ++i)
		printf("%02x", hash[i]);
	printf("\n");
}