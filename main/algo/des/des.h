#ifndef DES
#define DES

#include <stdint.h>

void des_generate_key(const char *password, uint64_t salt, uint8_t hash[16]);
uint64_t des_encrypt(uint64_t block, uint64_t key);

#endif