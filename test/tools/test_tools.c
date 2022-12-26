#include "test_tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io_tools.h"

#pragma region private

void print_ok(const char *test_group, const unsigned test_id)
{
	printf(TERM_GRN "[%s@%i] OK\n" TERM_RESET, test_group, test_id);
}

void print_ko_hash(const char *test_group, const unsigned test_id, const char *message, const char *actual_hash, const char *expected_hash)
{
	printf(TERM_RED);
	printf("[%s@%i] MESSAGE:  '%s'\n", test_group, test_id, message);
	printf("[%s@%i] ACTUAL:   '%s'\n", test_group, test_id, actual_hash);
	printf("[%s@%i] EXPECTED: '%s'\n", test_group, test_id, expected_hash);
	printf(TERM_RESET);
}

#pragma endregion

int test_result = 0;

const char *random_str(const unsigned max_length)
{
	const unsigned length = 1 + rand() % (max_length - 1);

	char *result = malloc(length + 1);
	result[length] = 0;

	for (unsigned i = 0; i < length; i++)
		result[i] = rand() % 127;

	return result;
}

void test_hash(const char *test_group, unsigned test_id, const char *message, unsigned hash_length, const char *actual_hash, const char *expected_hash)
{
	if (strncmp(actual_hash, expected_hash, hash_length) == 0)
	{
		print_ok(test_group, test_id);
	}
	else
	{
		print_ko_hash(test_group, test_id, message, actual_hash, expected_hash);
		test_result = 1;
	}
}