#include "md5_interface.h"
#include "md5.h"
#include <string.h>

void md5_string(const char *string, uint8_t *digest)
{
	struct md5_context context;

	md5_init(&context);
	md5_update(&context, string, strlen(string));
	md5_finalize(&context, digest);
}