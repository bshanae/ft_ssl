#ifndef DES_INTERFACE
#define DES_INTERFACE

#include <stdint.h>
#include <stddef.h>

#define DES_BLOCK_SIZE 8

size_t des_encrypted_size(size_t plain_size);

void des_generate_key_and_iv(const char *password, uint64_t salt, uint64_t *key, uint64_t *iv);

void des_encrypt_ecb(const void *plaintext, size_t plaintext_size, void *ciphertext, const uint64_t *key);
int des_decrypt_ecb(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key);

void des_encrypt_cbc(const void *plaintext, size_t plaintext_size, void *ciphertext, const uint64_t *key, const uint64_t *iv);
int des_decrypt_cbc(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key, const uint64_t *iv);

#endif