#include <printf.h>
#include <stdlib.h>
#include <string.h>
#include "io_tools.h"

const char *hex_to_str(uint8_t *data, unsigned size)
{
	const unsigned result_size = size * 2 + 1;

	char *result = malloc(result_size);
	result[result_size - 1] = 0;

	for (unsigned i = 0; i < size; ++i)
		sprintf(result + i * 2, "%02x", data[i]);

	return result;
}