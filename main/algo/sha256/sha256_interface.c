#include "sha256_interface.h"
#include "sha256.h"
#include "libft_standart.h"

void sha256_memory(const char *data, size_t data_length, uint8_t hash[SHA256_HASH_SIZE])
{
	struct sha256_context context;

	sha256_init(&context);
	sha256_update(&context, data, data_length);
	sha256_finalize(&context, hash);
}

void sha256_string(const char *string, uint8_t hash[SHA256_HASH_SIZE])
{
	sha256_memory(string, ft_strlen(string), hash);
}