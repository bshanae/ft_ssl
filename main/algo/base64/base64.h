#ifndef BASE64
#define BASE64

#include <stddef.h>

size_t base64_encoded_size(size_t message_size);
size_t base64_decoded_size(size_t encoded_size);

void base64_encode(char *out, const char *in, size_t size);
void base64_decode(char *out, const char *in, size_t size);

#endif