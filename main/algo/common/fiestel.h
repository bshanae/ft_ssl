#ifndef FIESTEL
#define FIESTEL

#include <stdint.h>

typedef uint32_t (*fiestel_f)(uint32_t v, uint64_t key);

uint64_t fiestel_encrypt(uint64_t block, uint64_t *keys, fiestel_f f, unsigned rounds);
void fiestel_decrypt(uint64_t block, uint64_t *keys, fiestel_f f, unsigned rounds);

#endif