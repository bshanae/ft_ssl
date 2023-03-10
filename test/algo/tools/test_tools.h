#ifndef TEST_TOOLS
#define TEST_TOOLS

extern int test_result;

const char *random_str(unsigned max_length);

void test_string(
	const char *test_group,
	unsigned test_id,
	unsigned length,
	const char *actual,
	const char *expected
);

void test_permutation(
	const char *test_group,
	unsigned test_id,
	const char *input,
	unsigned permutation_length,
	const char *actual_permutation,
	const char *expected_permutation
);

#endif