#include "sha256_interface.h"
#include "sha256.h"
#include <string.h>

void sha256_string(const char *string, uint8_t *digest)
{
	struct sha256_context context;

	sha256_init(&context);
	sha256_update(&context, string, strlen(string));
	sha256_finalize(&context, digest);
}