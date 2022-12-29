#include "sha256_tests.h"

#include <stdlib.h>
#include <CommonCrypto/CommonDigest.h>
#include <string.h>
#include "defines.h"
#include "algo/sha256/sha256_interface.h"
#include "tools/io_tools.h"
#include "tools/test_tools.h"

void sha256_test(const unsigned test_id, const char *input)
{
	uint8_t reference_hash[64];
	CC_SHA256(input, strlen(input), reference_hash);
	const char *reference_hash_str = hex_to_str(reference_hash, 64);

	uint8_t actual_hash[64];
	sha256_string(input, actual_hash);
	const char *actual_hash_str = hex_to_str(actual_hash, 64);

	test_permutation("sha256", test_id, input, 64, actual_hash_str, reference_hash_str);

	free((void *)actual_hash_str);
	free((void *)reference_hash_str);
}

void sha256_tests()
{
	sha256_test(1, "");
	sha256_test(2, "1");
	sha256_test(3, "123");
	sha256_test(4, "hello world");
	sha256_test(5, "qwertyuiopasdfghjklxcvbnm,cjhdsbcjhsbcjhbsdjchbsjhcbsjhbchsdbcjhsbdcjhbsdjhcbsjhdcbjshbdcjshbcdqwertyuiopasdfghjklxcvbnm,cj");

	for (int i = 0; i < RANDOM_TEST_SIZE; i++)
	{
		const char *random_message = random_str(1024);
		sha256_test(1000 + i, random_message);
		free((void *)random_message);
	}
}