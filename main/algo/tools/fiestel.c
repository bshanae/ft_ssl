#include "fiestel.h"

uint64_t fiestel_encrypt(const uint64_t block, const uint64_t *keys, fiestel_functor f, const unsigned rounds)
{
	uint32_t left = (block & 0xFFFFFFFF00000000ull) >> 32;
	uint32_t right = block & 0x00000000FFFFFFFFull;

	for (int i = 0; i < rounds; i++)
	{
		const uint32_t right_backup = right;

		right = left ^ f(right, keys[i]);
		left = right_backup;
	}

	return ((uint64_t)right << 32) | (uint64_t)left;
}

uint64_t fiestel_decrypt(const uint64_t block, const uint64_t *keys, fiestel_functor f, const unsigned rounds)
{
	uint32_t left = (block & 0xFFFFFFFF00000000ull) >> 32;
	uint32_t right = block & 0x00000000FFFFFFFFull;

	for (int i = (int)rounds - 1; i >= 0; i--)
	{
		const uint32_t right_backup = right;

		right = left ^ f(right, keys[i]);
		left = right_backup;
	}

	return ((uint64_t)right << 32) | (uint64_t)left;
}