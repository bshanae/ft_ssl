#ifndef TEST_TOOLS
#define TEST_TOOLS

extern int test_result;

const char *random_str(unsigned max_length);
void test_hash(const char *test_group, unsigned test_id, const char *message, unsigned hash_length, const char *actual_hash, const char *expected_hash);

#endif