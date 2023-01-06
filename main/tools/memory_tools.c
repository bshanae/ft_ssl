#include "memory_tools.h"

int is_little_endian()
{
	int test = 1;
	return *(char *)&test == 1;
}

uint32_t to_bigendian_32(const uint32_t value)
{
	if (is_little_endian())
		return SWAP_32(value);

	return value;
}

uint64_t to_bigendian_64(const uint64_t value)
{
	if (is_little_endian())
		return SWAP_64(value);

	return value;
}