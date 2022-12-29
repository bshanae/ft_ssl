#include "memory_tools.h"

int is_little_endian()
{
	int test = 1;
	return *(char *)&test == 1;
}