#include "md5_tests.h"

#include <stdlib.h>
#include <CommonCrypto/CommonDigest.h>
#include <string.h>
#include "defines.h"
#include "algo/md5/md5_interface.h"
#include "tools/io_tools.h"
#include "tools/test_tools.h"

void md5_test(const unsigned test_id, const char *input)
{
	uint8_t reference_hash[16];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	CC_MD5(input, strlen(input), reference_hash);
#pragma GCC diagnostic pop
	const char *reference_hash_str = hex_to_str(reference_hash, 16);

	uint8_t actual_hash[16];
	md5_string(input, actual_hash);
	const char *actual_hash_str = hex_to_str(actual_hash, 16);

	test_permutation("md5", test_id, input, 16, actual_hash_str, reference_hash_str);

	free((void *) reference_hash_str);
	free((void *) actual_hash_str);
}

void md5_tests()
{
	md5_test(0, "");
	md5_test(1, "a");
	md5_test(2, "abc");
	md5_test(3, "message digest");
	md5_test(4, "abcdefghijklmnopqrstuvwxyz");
	md5_test(5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	md5_test(6, "12345678901234567890123456789012345678901234567890123456789012345678901234567890");

	for (int i = 0; i < RANDOM_TEST_SIZE; i++)
	{
		const char *random_message = random_str(1024);
		md5_test(1000 + i, random_message);
		free((void *)random_message);
	}
}