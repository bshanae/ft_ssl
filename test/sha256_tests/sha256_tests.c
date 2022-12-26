#include "sha256_tests.h"

#include <stdlib.h>
#include <CommonCrypto/CommonDigest.h>
#include <string.h>
#include "sha256/sha256_interface.h"
#include "tools/io_tools.h"
#include "tools/test_tools.h"

void sha256_test(const unsigned test_id, const char *input)
{
	uint8_t reference_digest[64];
	CC_SHA256(input, strlen(input), reference_digest);
	const char *reference_digest_str = hex_to_str(reference_digest, 64);

	uint8_t actual_digest[64];
	sha256_string(input, actual_digest);
	const char *actual_digest_str = hex_to_str(actual_digest, 64);

	test_string("sha256", test_id, 64, actual_digest_str, reference_digest_str);

	free((void *)actual_digest_str);
	free((void *)reference_digest_str);
}

void sha256_tests()
{
	sha256_test(1, "");
	sha256_test(2, "1");
	sha256_test(3, "123");
	sha256_test(4, "hello world");
	sha256_test(5, "qwertyuiopasdfghjklxcvbnm,cjhdsbcjhsbcjhbsdjchbsjhcbsjhbchsdbcjhsbdcjhbsdjhcbsjhdcbjshbdcjshbcdqwertyuiopasdfghjklxcvbnm,cj");

	for (int i = 0; i < 100; i++)
	{
		const char *random_message = random_str(1024);
		sha256_test(1000 + i, random_message);
		free((void *)random_message);
	}
}