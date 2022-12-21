#include "md5/md5_interface.h"
#include "tools/io_tools.h"

void test(const char *string)
{
	uint8_t digest[16];
	md5_string(string, digest);

	print_hex(digest, 16);
}

int main()
{
	test("");
	test("a");
	test("abc");
	test("message digest");
	test("abcdefghijklmnopqrstuvwxyz");
	test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	test("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}