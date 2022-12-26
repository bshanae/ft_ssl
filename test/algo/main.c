#include "tools/test_tools.h"
#include "md5_tests/md5_tests.h"
#include "sha256_tests/sha256_tests.h"

int main()
{
	md5_tests();
	sha256_tests();

	return test_result;
}