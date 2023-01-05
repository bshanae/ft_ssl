#ifndef MD5_INTERFACE
#define MD5_INTERFACE

#include <stdint.h>
#include <stddef.h>

void md5_memory(const char *memory, size_t size, uint8_t hash[16]);
void md5_string(const char *string, uint8_t hash[16]);

#endif
