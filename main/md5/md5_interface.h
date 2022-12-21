#ifndef MD5_INTERFACE
#define MD5_INTERFACE

#include <stdint.h>

void md5_string(const char *string, uint8_t *digest);

#endif
