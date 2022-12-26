#ifndef MD5
#define MD5

#include <stddef.h>
#include "stdint.h"

struct md5_context
{
	uint64_t size;
	uint32_t state[4];
	uint8_t cache[64];
};

void md5_init(struct md5_context *context);
void md5_update(struct md5_context *context, const void *input, size_t input_size);
void md5_finalize(struct md5_context *context, unsigned char hash[16]);

#endif