#include "base64.h"

#include <stdint.h>
#include "tools/memory_tools.h"
#include "tools/math_tools.h"

static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int inverse_charset[] =
{
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, // .
	0, //
	0, // !
	0, // "
	0, // #
	0, // $
	0, // %
	0, // &
	0, // '
	0, // (
	0, // )
	0, // *
	62, // +
	0, // ,
	0, // -
	0, // .
	63, // /
	52, // 0
	53, // 1
	54, // 2
	55, // 3
	56, // 4
	57, // 5
	58, // 6
	59, // 7
	60, // 8
	61, // 9
	0, // :
	0, // ;
	0, // <
	0, // =
	0, // >
	0, // ?
	0, // @
	0, // A
	1, // B
	2, // C
	3, // D
	4, // E
	5, // F
	6, // G
	7, // H
	8, // I
	9, // J
	10, // K
	11, // L
	12, // M
	13, // N
	14, // O
	15, // P
	16, // Q
	17, // R
	18, // S
	19, // T
	20, // U
	21, // V
	22, // W
	23, // X
	24, // Y
	25, // Z
	0, // [
	0, // .
	0, // ]
	0, // ^
	0, // _
	0, // `
	26, // a
	27, // b
	28, // c
	29, // d
	30, // e
	31, // f
	32, // g
	33, // h
	34, // i
	35, // j
	36, // k
	37, // l
	38, // m
	39, // n
	40, // o
	41, // p
	42, // q
	43, // r
	44, // s
	45, // t
	46, // u
	47, // v
	48, // w
	49, // x
	50, // y
	51, // z
	0, // {
	0, // |
	0, // }
	0, // ~
	0  // .
};

size_t base64_encoded_size(size_t message_size)
{
	const size_t base_size = ROUND_UP(message_size * 4 / 3, 4);
	const size_t newline_count = base_size / 64;

	return base_size + newline_count;
}

size_t base64_decoded_size(size_t encoded_size)
{
	const size_t newline_count = encoded_size / 64;

	return encoded_size * 3 / 4 - newline_count;
}

void base64_encode(char *out, const char *in, size_t size, int put_newline)
{
	int j = 0;
	for (int i = 0; i < size; i += 3)
	{
		uint32_t chars = to_bigendian_32(*(uint32_t *)(in + i));

		out[j++] = charset[chars >> 26];
		out[j++] = charset[(chars >> 20) & 0x3f];
		out[j++] = charset[(chars >> 14) & 0x3f];
		out[j++] = charset[(chars >> 8) & 0x3f];

		if (put_newline && j % 64 == 0)
			out[j++] = '\n';
	}

	switch (size % 3)
	{
		case 1:
			out[j - 2] = '=';

		case 2:
			out[j - 1] = '=';
	}
}

void base64_decode(char *out, const char *in, size_t size)
{
	for (int i = 0, j = 0; i < size; i += 4)
	{
		const unsigned padding_count = (in[i + 2] == '=') + (in[i + 3] == '=');

		uint32_t chars;
		chars = inverse_charset[in[i + 0]] << 26;
		chars |= inverse_charset[in[i + 1]] << 20;
		chars |= inverse_charset[in[i + 2]] << 14;
		chars |= inverse_charset[in[i + 3]] << 8;

		chars = to_bigendian_32(chars);

		char *chars_ptr = (char *)&chars;
		out[j++] = chars_ptr[0];
		if (padding_count < 2)
			out[j++] = chars_ptr[1];
		if (padding_count < 1)
			out[j++] = chars_ptr[2];
	}
}