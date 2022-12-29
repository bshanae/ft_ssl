#include "tools/test_tools.h"
#include "md5_tests/md5_tests.h"
#include "sha256_tests/sha256_tests.h"
#include "base64_tests/base64_tests.h"

int main()
{
	md5_tests();
	sha256_tests();
	base64_tests();

	return test_result;
}