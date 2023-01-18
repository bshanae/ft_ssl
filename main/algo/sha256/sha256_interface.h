#ifndef SHA256_INTERFACE
#define SHA256_INTERFACE

#include <stdint.h>
#include <stddef.h>

#define SHA256_HASH_SIZE 32

void sha256_memory(const char *data, size_t data_length, uint8_t hash[SHA256_HASH_SIZE]);
void sha256_string(const char *string, uint8_t hash[SHA256_HASH_SIZE]);

#endif
