#ifndef BASE64
#define BASE64

#include <stddef.h>

size_t base64_encoded_size(size_t message_size);
size_t base64_max_decoded_size(size_t encoded_size);

void base64_encode(const char *plain, size_t plain_size, char *encoded, int put_newline);
void base64_decode(const char *encoded, size_t encoded_size, char *decoded, size_t *decoded_size);

#endif