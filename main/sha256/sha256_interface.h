#ifndef SHA256_INTERFACE
#define SHA256_INTERFACE

#include <stdint.h>

void sha256_string(const char *string, uint8_t *digest);

#endif
