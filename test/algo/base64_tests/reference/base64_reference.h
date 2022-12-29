// https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
#ifndef BASE64_REFERENCE
#define BASE64_REFERENCE

#include <stdlib.h>

char *ref_base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);

#endif