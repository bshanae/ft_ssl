#include "fiestel.h"

uint64_t fiestel_encrypt(uint64_t block, uint64_t *keys, fiestel_f f, unsigned rounds)
{
	uint32_t left = (block & 0xFFFFFFFF00000000ull) >> 32;
	uint32_t right = block & 0x00000000FFFFFFFFull;

	for (int i = 0; i < rounds; i++)
	{
		uint32_t temp = right;
		right = left ^ f(right, keys[i]);
		left = temp;
	}

	return ((uint64_t)right << 32) | (uint64_t)left;
}

void fiestel_decrypt(uint64_t block, uint64_t *keys, fiestel_f f, unsigned rounds)
{
	uint32_t left = block & 0x00000000FFFFFFFFull;
	uint32_t right = block & 0xFFFFFFFF00000000ull;

	for (int i = (int)rounds - 1; i >= 0; i--)
	{
		uint32_t temp = left ^ f(right, keys[i]);
		left = right;
		right = temp;
	}
}