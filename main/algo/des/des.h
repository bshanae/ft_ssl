#ifndef DES
#define DES

#include <stdint.h>

uint64_t des_encrypt(uint64_t block, uint64_t key);
uint64_t des_decrypt(uint64_t block, uint64_t key);

#endif