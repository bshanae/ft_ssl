#ifndef DES_INTERFACE
#define DES_INTERFACE

#include <stdint.h>
#include <stddef.h>

#define DES_BLOCK_SIZE 8

size_t des_encrypted_size(size_t plain_size);

void des_generate_key(const char *password, uint64_t salt, uint8_t hash[16]);
void des_ecb(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key);
void des_cbc(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key, const uint64_t *iv);

#endif