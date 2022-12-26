#ifndef SHA256
#define SHA256

#include <stddef.h>
#include "stdint.h"

struct sha256_context
{
	uint64_t size;
	uint64_t bitlen;
	uint8_t cache[64];
	uint32_t state[8];
};

void sha256_init(struct sha256_context *context);
void sha256_update(struct sha256_context *context, const void *input, size_t input_size);
void sha256_finalize(struct sha256_context *context, uint8_t digest[32]);

#endif