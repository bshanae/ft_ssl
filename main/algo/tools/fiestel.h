#ifndef FIESTEL
#define FIESTEL

#include <stdint.h>

typedef uint32_t (*fiestel_functor)(uint32_t v, uint64_t key);

uint64_t fiestel_encrypt(uint64_t block, const uint64_t *keys, fiestel_functor f, unsigned rounds);
uint64_t fiestel_decrypt(uint64_t block, const uint64_t *keys, fiestel_functor f, unsigned rounds);

#endif