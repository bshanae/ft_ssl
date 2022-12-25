#ifndef TEST_TOOLS
#define TEST_TOOLS

extern int test_result;

const char *random_str(unsigned max_length);
void test_string(const char *test_group, const unsigned test_id, unsigned length, const char *actual, const char *expected);

#endif