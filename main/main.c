#include "md5/md5_interface.h"
#include "tools/io_tools.h"
#include "sha256/sha256_interface.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

void md5_test(const char *input, const char *expected_digest)
{
	uint8_t digest[16];
	md5_string(input, digest);

	const char *digest_str = hex_to_str(digest, 16);

	if (memcmp(expected_digest, digest_str, 16) != 0)
	{
		printf("ERROR!\n");
		printf("Input: %s\n", input);
		printf("Expected digest: %s\n", expected_digest);
		printf("Actual digest:   %s\n", digest_str);
	}

	free(digest_str);
}

void md5_test_suite()
{
	md5_test(
		"",
		"d41d8cd98f00b204e9800998ecf8427e"
	);

	md5_test(
		"a",
		"0cc175b9c0f1b6a831c399e269772661"
	);

	md5_test(
		"abc",
		"900150983cd24fb0d6963f7d28e17f72"
	);

	md5_test(
		"message digest",
		"f96b697d7cb7938d525a2f31aaf161d0"
	);

	md5_test(
		"abcdefghijklmnopqrstuvwxyz",
		"c3fcd3d76192e4007dfb496cca67e13b"
	);

	md5_test(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"d174ab98d277d9f5a5611c2c9f419d9f"
	);

	md5_test(
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		"57edf4a22be3c955ac49da2e2107b67a"
	);
}

void sha256_test(const char *input, const char *expected_digest)
{
	uint8_t digest[32];
	sha256_string(input, digest);

	const char *digest_str = hex_to_str(digest, 32);

	if (memcmp(expected_digest, digest_str, 32) != 0)
	{
		printf("ERROR!\n");
		printf("Input: %s\n", input);
		printf("Expected digest: %s\n", expected_digest);
		printf("Actual digest:   %s\n", digest_str);
	}

	free(digest_str);
}

void sha256_test_suite()
{
	sha256_test(
		"",
		"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
	);

	sha256_test(
		"1",
		"6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b"
	);

	sha256_test(
		"123",
		"a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3"
	);

	sha256_test(
		"hello world",
		"b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9"
	);

	sha256_test(
		"qwertyuiopasdfghjklxcvbnm,cjhdsbcjhsbcjhbsdjchbsjhcbsjhbchsdbcjhsbdcjhbsdjhcbsjhdcbjshbdcjshbcdqwertyuiopasdfghjklxcvbnm,cj",
		"1d77a125d4d3bf082831afbfcb2ee2329fb9d8f9eb0bef096c1bc76e336dfe9a"
	);
}

int main()
{
	md5_test_suite();
	sha256_test_suite();
}