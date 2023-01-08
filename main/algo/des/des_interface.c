#include "des_interface.h"

#include "libft_standart.h"
#include "tools/memory_tools.h"
#include "algo/tools/padding.h"
#include "algo/md5/md5_interface.h"
#include "des.h"

size_t des_encrypted_size(const size_t plain_size)
{
	if (plain_size % DES_BLOCK_SIZE == 0)
		return plain_size + DES_BLOCK_SIZE;
	else
		return (plain_size / DES_BLOCK_SIZE + 1) * DES_BLOCK_SIZE;
}

void des_generate_key(const char *password, uint64_t salt, uint8_t hash[16])
{
	const int password_length = (int)ft_strlen(password);
	const int password_with_salt_length = password_length + 8;

	char *password_with_salt = malloc(password_with_salt_length + 1);
	ft_strcpy(password_with_salt, password);
	ft_memcpy(password_with_salt + password_length, &salt, 8);
	password_with_salt[password_with_salt_length] = '\0';

	md5_memory(password_with_salt, 12, hash);

	free(password_with_salt);
}

void des_ecb(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key)
{
	unsigned i = 0;
	for (; i + DES_BLOCK_SIZE <= size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(plaintext + i));
		*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(block, *key));
	}

	uint8_t padding_block8[DES_BLOCK_SIZE];
	ft_memcpy(padding_block8, plaintext + i, size - i);
	pad_block(padding_block8, size - i, DES_BLOCK_SIZE);

	const uint64_t padding_block64 = to_bigendian_64(*(uint64_t *)padding_block8);

	*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(padding_block64, *key));
}

void des_cbc(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key, const uint64_t *iv)
{
	uint64_t previous_block = *iv;

	unsigned i = 0;
	for (; i + DES_BLOCK_SIZE <= size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(plaintext + i));
		const uint64_t encrypted_block = to_bigendian_64(des_encrypt(block ^ previous_block, *key));

		*(uint64_t *)(ciphertext + i) = encrypted_block;
		previous_block = to_bigendian_64(encrypted_block);
	}

	uint8_t padding_block8[DES_BLOCK_SIZE];
	ft_memcpy(padding_block8, plaintext + i, size - i);
	pad_block(padding_block8, size - i, DES_BLOCK_SIZE);

	const uint64_t padding_block64 = to_bigendian_64(*(uint64_t *) padding_block8);

	*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(padding_block64 ^ previous_block, *key));
}