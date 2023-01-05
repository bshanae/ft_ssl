#include "md5_interface.h"

#include "md5.h"
#include "libft_standart.h"

void md5_memory(const char *memory, const size_t size, uint8_t hash[16])
{
	struct md5_context context;

	md5_init(&context);
	md5_update(&context, memory, size);
	md5_finalize(&context, hash);
}

void md5_string(const char *string, uint8_t hash[16])
{
	md5_memory(string, ft_strlen(string), hash);
}