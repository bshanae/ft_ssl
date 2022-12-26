#include "sha256.h"
#include "tools/memory_tools.h"
#include "libft_standart.h"

#pragma region private

#define INITIAL_A 0x6a09e667
#define INITIAL_B 0xbb67ae85
#define INITIAL_C 0x3c6ef372
#define INITIAL_D 0xa54ff53a
#define INITIAL_E 0x510e527f
#define INITIAL_F 0x9b05688c
#define INITIAL_G 0x1f83d9ab
#define INITIAL_H 0x5be0cd19

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SSIG0(x) (ROTATE_RIGHT_32(x, 7) ^ ROTATE_RIGHT_32(x, 18) ^ ((x) >> 3))
#define SSIG1(x) (ROTATE_RIGHT_32(x, 17) ^ ROTATE_RIGHT_32(x, 19) ^ ((x) >> 10))
#define BSIG0(x) (ROTATE_RIGHT_32(x, 2) ^ ROTATE_RIGHT_32(x, 13) ^ ROTATE_RIGHT_32(x, 22))
#define BSIG1(x) (ROTATE_RIGHT_32(x, 6) ^ ROTATE_RIGHT_32(x, 11) ^ ROTATE_RIGHT_32(x, 25))

static const uint32_t k[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint8_t padding[64] =
{
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void sha256_transform(struct sha256_context *context, const uint8_t *input_message)
{
	// copy message

	uint32_t message[64];
	for (unsigned i = 0; i < 16; i++)
	{
		uint32_t block = *(uint32_t *) (input_message + i * 4);
		message[i] = SWAP_32(block);
	}

	// expand message

	for (unsigned i = 16; i < 64; ++i)
		message[i] = SSIG1(message[i - 2]) + message[i - 7] + SSIG0(message[i - 15]) + message[i - 16];

	// apply the function

	uint32_t a = context->state[0];
	uint32_t b = context->state[1];
	uint32_t c = context->state[2];
	uint32_t d = context->state[3];
	uint32_t e = context->state[4];
	uint32_t f = context->state[5];
	uint32_t g = context->state[6];
	uint32_t h = context->state[7];

	for (unsigned i = 0; i < 64; i++)
	{
		const uint32_t t1 = h + BSIG1(e) + CH(e, f, g) + k[i] + message[i];
		const uint32_t t2 = BSIG0(a) + MAJ(a, b, c);

		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;
	context->state[4] += e;
	context->state[5] += f;
	context->state[6] += g;
	context->state[7] += h;
}

void sha256_update__(struct sha256_context *context, const uint8_t *input, size_t input_size)
{
	const size_t used_in_cache = context->size % 64;
	const size_t free_in_cache = 64 - used_in_cache;

	context->size += input_size;

	if (input_size >= free_in_cache)
	{
		// fill cache and process its content
		ft_memcpy(context->cache + used_in_cache, input, free_in_cache);
		context->bitlen += 64 * 8;
		sha256_transform(context, context->cache);

		// process input by 64 bytes block
		size_t written = free_in_cache;
		for (; written + 63 < input_size; written += 64)
		{
			context->bitlen += 64 * 8;
			sha256_transform(context, &input[written]);
		}

		// copy leftover data into cache
		ft_memcpy(context->cache, input + written, input_size - written);
	}
	else
	{
		// write input to cache
		// in next update call or in finalize call the cache will be processed
		ft_memcpy(context->cache + used_in_cache, input, input_size);
	}
}

#pragma endregion

void sha256_init(struct sha256_context *context)
{
	context->size = 0;
	context->bitlen = 0;

	context->state[0] = INITIAL_A;
	context->state[1] = INITIAL_B;
	context->state[2] = INITIAL_C;
	context->state[3] = INITIAL_D;
	context->state[4] = INITIAL_E;
	context->state[5] = INITIAL_F;
	context->state[6] = INITIAL_G;
	context->state[7] = INITIAL_H;
}

void sha256_update(struct sha256_context *context, const void *input, size_t input_size)
{
	sha256_update__(context, input, input_size);
}

void sha256_finalize(struct sha256_context *context, uint8_t hash[32])
{
	// save size

	uint64_t size_in_bits = context->size * 8;

	// write padding

	const unsigned offset = context->size % 64;
	const unsigned padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

	sha256_update(context, padding, padding_length);

	// write size (in big endian)

	size_in_bits = SWAP_64(size_in_bits);
	sha256_update(context, &size_in_bits, 8);

	// write hash (in big endian)

	uint32_t *hash32 = (uint32_t *)hash;
	for (int i = 0; i < 8; i++)
		hash32[i] = SWAP_32(context->state[i]);
}