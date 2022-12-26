#include "md5.h"
#include "tools/memory_tools.h"
#include "libft_standart.h"

#pragma region private

#define INITIAL_A 0x67452301
#define INITIAL_B 0xefcdab89
#define INITIAL_C 0x98badcfe
#define INITIAL_D 0x10325476

#define T01 0xd76aa478
#define T02 0xe8c7b756
#define T03 0x242070db
#define T04 0xc1bdceee
#define T05 0xf57c0faf
#define T06 0x4787c62a
#define T07 0xa8304613
#define T08 0xfd469501
#define T09 0x698098d8
#define T10 0x8b44f7af
#define T11 0xffff5bb1
#define T12 0x895cd7be
#define T13 0x6b901122
#define T14 0xfd987193
#define T15 0xa679438e
#define T16 0x49b40821
#define T17 0xf61e2562
#define T18 0xc040b340
#define T19 0x265e5a51
#define T20 0xe9b6c7aa
#define T21 0xd62f105d
#define T22 0x02441453
#define T23 0xd8a1e681
#define T24 0xe7d3fbc8
#define T25 0x21e1cde6
#define T26 0xc33707d6
#define T27 0xf4d50d87
#define T28 0x455a14ed
#define T29 0xa9e3e905
#define T30 0xfcefa3f8
#define T31 0x676f02d9
#define T32 0x8d2a4c8a
#define T33 0xfffa3942
#define T34 0x8771f681
#define T35 0x6d9d6122
#define T36 0xfde5380c
#define T37 0xa4beea44
#define T38 0x4bdecfa9
#define T39 0xf6bb4b60
#define T40 0xbebfbc70
#define T41 0x289b7ec6
#define T42 0xeaa127fa
#define T43 0xd4ef3085
#define T44 0x04881d05
#define T45 0xd9d4d039
#define T46 0xe6db99e5
#define T47 0x1fa27cf8
#define T48 0xc4ac5665
#define T49 0xf4292244
#define T50 0x432aff97
#define T51 0xab9423a7
#define T52 0xfc93a039
#define T53 0x655b59c3
#define T54 0x8f0ccc92
#define T55 0xffeff47d
#define T56 0x85845dd1
#define T57 0x6fa87e4f
#define T58 0xfe2ce6e0
#define T59 0xa3014314
#define T60 0x4e0811a1
#define T61 0xf7537e82
#define T62 0xbd3af235
#define T63 0x2ad7d2bb
#define T64 0xeb86d391

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

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

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define FF(a, b, c, d, x, s, t)                      \
{                                                    \
    (a) += F((b), (c), (d)) + (x) + (uint32_t)(t);   \
    (a) = ROTATE_LEFT_32((a), (s));                  \
    (a) += (b);                                      \
}

#define GG(a, b, c, d, x, s, t)                      \
{                                                    \
    (a) += G((b), (c), (d)) + (x) + (uint32_t)(t);   \
    (a) = ROTATE_LEFT_32((a), (s));                  \
    (a) += (b);                                      \
}

#define HH(a, b, c, d, x, s, t)                      \
{                                                    \
    (a) += H((b), (c), (d)) + (x) + (uint32_t)(t);   \
    (a) = ROTATE_LEFT_32((a), (s));                  \
    (a) += (b);                                      \
}

#define II(a, b, c, d, x, s, t)                      \
{                                                    \
    (a) += I((b), (c), (d)) + (x) + (uint32_t)(t);   \
    (a) = ROTATE_LEFT_32((a), (s));                  \
    (a) += (b);                                      \
}

void md5_transform(uint32_t state[4], const void *input)
{
	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];

	uint32_t x[16];
	ft_memcpy(x, input, 64);

	// round 1
	FF(a, b, c, d, x[ 0], S11, T01)
	FF(d, a, b, c, x[ 1], S12, T02)
	FF(c, d, a, b, x[ 2], S13, T03)
	FF(b, c, d, a, x[ 3], S14, T04)
	FF(a, b, c, d, x[ 4], S11, T05)
	FF(d, a, b, c, x[ 5], S12, T06)
	FF(c, d, a, b, x[ 6], S13, T07)
	FF(b, c, d, a, x[ 7], S14, T08)
	FF(a, b, c, d, x[ 8], S11, T09)
	FF(d, a, b, c, x[ 9], S12, T10)
	FF(c, d, a, b, x[10], S13, T11)
	FF(b, c, d, a, x[11], S14, T12)
	FF(a, b, c, d, x[12], S11, T13)
	FF(d, a, b, c, x[13], S12, T14)
	FF(c, d, a, b, x[14], S13, T15)
	FF(b, c, d, a, x[15], S14, T16)

	// round 2
	GG(a, b, c, d, x[ 1], S21, T17)
	GG(d, a, b, c, x[ 6], S22, T18)
	GG(c, d, a, b, x[11], S23, T19)
	GG(b, c, d, a, x[ 0], S24, T20)
	GG(a, b, c, d, x[ 5], S21, T21)
	GG(d, a, b, c, x[10], S22, T22)
	GG(c, d, a, b, x[15], S23, T23)
	GG(b, c, d, a, x[ 4], S24, T24)
	GG(a, b, c, d, x[ 9], S21, T25)
	GG(d, a, b, c, x[14], S22, T26)
	GG(c, d, a, b, x[ 3], S23, T27)
	GG(b, c, d, a, x[ 8], S24, T28)
	GG(a, b, c, d, x[13], S21, T29)
	GG(d, a, b, c, x[ 2], S22, T30)
	GG(c, d, a, b, x[ 7], S23, T31)
	GG(b, c, d, a, x[12], S24, T32)

	// round 3
	HH(a, b, c, d, x[ 5], S31, T33)
	HH(d, a, b, c, x[ 8], S32, T34)
	HH(c, d, a, b, x[11], S33, T35)
	HH(b, c, d, a, x[14], S34, T36)
	HH(a, b, c, d, x[ 1], S31, T37)
	HH(d, a, b, c, x[ 4], S32, T38)
	HH(c, d, a, b, x[ 7], S33, T39)
	HH(b, c, d, a, x[10], S34, T40)
	HH(a, b, c, d, x[13], S31, T41)
	HH(d, a, b, c, x[ 0], S32, T42)
	HH(c, d, a, b, x[ 3], S33, T43)
	HH(b, c, d, a, x[ 6], S34, T44)
	HH(a, b, c, d, x[ 9], S31, T45)
	HH(d, a, b, c, x[12], S32, T46)
	HH(c, d, a, b, x[15], S33, T47)
	HH(b, c, d, a, x[ 2], S34, T48)

	// round 4
	II(a, b, c, d, x[ 0], S41, T49)
	II(d, a, b, c, x[ 7], S42, T50)
	II(c, d, a, b, x[14], S43, T51)
	II(b, c, d, a, x[ 5], S44, T52)
	II(a, b, c, d, x[12], S41, T53)
	II(d, a, b, c, x[ 3], S42, T54)
	II(c, d, a, b, x[10], S43, T55)
	II(b, c, d, a, x[ 1], S44, T56)
	II(a, b, c, d, x[ 8], S41, T57)
	II(d, a, b, c, x[15], S42, T58)
	II(c, d, a, b, x[ 6], S43, T59)
	II(b, c, d, a, x[13], S44, T60)
	II(a, b, c, d, x[ 4], S41, T61)
	II(d, a, b, c, x[11], S42, T62)
	II(c, d, a, b, x[ 2], S43, T63)
	II(b, c, d, a, x[ 9], S44, T64)

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

#pragma endregion

void md5_init(struct md5_context *context)
{
	context->size = 0;

	context->state[0] = INITIAL_A;
	context->state[1] = INITIAL_B;
	context->state[2] = INITIAL_C;
	context->state[3] = INITIAL_D;
}

void md5_update(struct md5_context *context, const void *input, size_t input_size)
{
	const size_t used_in_cache = context->size % 64;
	const size_t free_in_cache = 64 - used_in_cache;

	context->size += input_size;

	if (input_size >= free_in_cache)
	{
		// fill cache and process its content
		ft_memcpy(context->cache + used_in_cache, input, free_in_cache);
		md5_transform(context->state, context->cache);

		// process input by 64 bytes block
		size_t written = free_in_cache;
		for (; written + 63 < input_size; written += 64)
			md5_transform(context->state, &input[written]);

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

void md5_finalize(struct md5_context *context, unsigned char digest[16])
{
	// save size

	uint64_t size_in_bits = context->size * 8;

	// write padding

	const unsigned offset = context->size % 64;
	const unsigned padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

	md5_update(context, padding, padding_length);

	// write size

	md5_update(context, &size_in_bits, 8);

	// move the result into digest

	for (unsigned int i = 0; i < 4; i++)
	{
		digest[(i * 4) + 0] = (uint8_t)((context->state[i] & 0x000000FF));
		digest[(i * 4) + 1] = (uint8_t)((context->state[i] & 0x0000FF00) >> 8);
		digest[(i * 4) + 2] = (uint8_t)((context->state[i] & 0x00FF0000) >> 16);
		digest[(i * 4) + 3] = (uint8_t)((context->state[i] & 0xFF000000) >> 24);
	}
}