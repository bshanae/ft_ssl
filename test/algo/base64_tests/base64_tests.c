#include "base64_tests.h"

#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "algo/base64/base64.c"
#include "tools/test_tools.h"
#include "reference/base64_reference.h"

void base64_encoding_test(const unsigned test_id, const char *input)
{
	size_t reference_size;
	char *reference = ref_base64_encode(input, strlen(input), &reference_size);

	const size_t result_size = base64_encoded_size(strlen(input));
	char *result = malloc(result_size);
	bzero(result, result_size);
	base64_encode(result, input, strlen(input));

	test_permutation(
		"base64",
		test_id,
		input,
		reference_size,
		result,
		reference
	);

	free(reference);
	free(result);
}

void base64_round_test(const unsigned test_id, const char *input)
{
	const size_t encoded_size = base64_encoded_size(strlen(input));
	char *encoded = malloc(encoded_size);
	bzero(encoded, encoded_size);
	base64_encode(encoded, input, strlen(input));

	const size_t decoded_size = base64_decoded_size(encoded_size);
	char *decoded = malloc(encoded_size);
	bzero(decoded, decoded_size);
	base64_decode(decoded, encoded, encoded_size - 1);

	test_string(
		"base64",
		test_id,
		strlen(input),
		decoded,
		input
	);

	free(encoded);
	free(decoded);
}

void base64_tests()
{
	base64_encoding_test(1, "");
	base64_encoding_test(2, "a");
	base64_encoding_test(3, "abc");
	base64_encoding_test(4, "abcd");
	base64_encoding_test(5, "hello world");
	base64_encoding_test(6, "\x01\x02\x05\x04\x05");

	for (int i = 0; i < 100; i++)
	{
		const char *str = random_str(1024);
		base64_encoding_test(100 + i, str);
		free((void *)str);
	}

	base64_round_test(1001, "");
	base64_round_test(1002, "1");
	base64_round_test(1003, "123");
	base64_round_test(1005, "hello world");
	base64_round_test(1006, "\x01\x02\x05\x04\x05");

	for (int i = 0; i < RANDOM_TEST_SIZE; i++)
	{
		const char *str = random_str(1024);
		base64_round_test(1000 + i, str);
		free((void *)str);
	}
}