#include "des_interface.h"

#include "libft_standart.h"
#include "tools/memory_tools.h"
#include "tools/io_tools.h"
#include "algo/tools/padding.h"
#include "algo/sha256/sha256_interface.h"
#include "des.h"

size_t des_encrypted_size(const size_t plain_size)
{
	if (plain_size % DES_BLOCK_SIZE == 0)
		return plain_size + DES_BLOCK_SIZE;
	else
		return (plain_size / DES_BLOCK_SIZE + 1) * DES_BLOCK_SIZE;
}

void des_generate_key_and_iv(const char *password, const uint64_t salt, uint64_t *key, uint64_t *iv)
{
	uint64_t hash[SHA256_HASH_SIZE / 8];

	const int password_length = (int)ft_strlen(password);
	const int password_with_salt_length = password_length + 8;

	char *password_with_salt = malloc(password_with_salt_length);
	ft_strcpy(password_with_salt, password);
	ft_memcpy(password_with_salt + password_length, &salt, 8);

	sha256_memory(password_with_salt, password_with_salt_length, (uint8_t *)&hash);

	free(password_with_salt);

	*key = to_bigendian_64(hash[0]);
	*iv = to_bigendian_64(hash[1]);
}

void des_encrypt_ecb(const void *plaintext, const size_t plaintext_size, void *ciphertext, const uint64_t *key)
{
	unsigned i = 0;
	for (; i + DES_BLOCK_SIZE <= plaintext_size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(plaintext + i));
		*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(block, *key));
	}

	uint8_t padding_block8[DES_BLOCK_SIZE];
	ft_memcpy(padding_block8, plaintext + i, plaintext_size - i);
	pad_block(padding_block8, plaintext_size - i, DES_BLOCK_SIZE);

	const uint64_t padding_block64 = to_bigendian_64(*(uint64_t *)padding_block8);

	*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(padding_block64, *key));
}

int des_decrypt_ecb(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key)
{
	if (ciphertext_size % DES_BLOCK_SIZE != 0)
	{
		print_error("Invalid ciphertext size.");
		return 1;
	}

	unsigned i = 0;
	for (; i < ciphertext_size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(ciphertext + i));
		*(uint64_t *)(plaintext + i) = to_bigendian_64(des_decrypt(block, *key));
	}

	const uint8_t padding_size = *(uint8_t *)(plaintext + i - 1);
	if (padding_size == 0 || padding_size >= i - 1)
	{
		print_error("Decryption error.");
		return 1;
	}
	*plaintext_size = i - padding_size;

	return 0;
}

void des_encrypt_cbc(const void *plaintext, size_t plaintext_size, void *ciphertext, const uint64_t *key, const uint64_t *iv)
{
	uint64_t previous_block = *iv;

	unsigned i = 0;
	for (; i + DES_BLOCK_SIZE <= plaintext_size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(plaintext + i));
		const uint64_t encrypted_block = to_bigendian_64(des_encrypt(block ^ previous_block, *key));

		*(uint64_t *)(ciphertext + i) = encrypted_block;
		previous_block = to_bigendian_64(encrypted_block);
	}

	uint8_t padding_block8[DES_BLOCK_SIZE];
	ft_memcpy(padding_block8, plaintext + i, plaintext_size - i);
	pad_block(padding_block8, plaintext_size - i, DES_BLOCK_SIZE);

	const uint64_t padding_block64 = to_bigendian_64(*(uint64_t *) padding_block8);

	*(uint64_t *)(ciphertext + i) = to_bigendian_64(des_encrypt(padding_block64 ^ previous_block, *key));
}

int des_decrypt_cbc(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key, const uint64_t *iv)
{
	if (ciphertext_size % DES_BLOCK_SIZE != 0)
	{
		print_error("Invalid ciphertext size.");
		return 1;
	}

	uint64_t previous_block = to_bigendian_64(*iv);

	unsigned i = 0;
	for (; i < ciphertext_size; i += DES_BLOCK_SIZE)
	{
		const uint64_t block = to_bigendian_64(*(uint64_t *)(ciphertext + i));
		const uint64_t decrypted_block = to_bigendian_64(des_decrypt(block, *key));

		*(uint64_t *)(plaintext + i) = decrypted_block ^ previous_block;
		previous_block = to_bigendian_64(block);
	}

	const uint8_t padding_size = *(uint8_t *)(plaintext + i - 1);
	if (padding_size == 0 || padding_size >= i - 1)
	{
		print_error("Decryption error.");
		return 1;
	}
	*plaintext_size = i - 1 - padding_size;

	return 0;
}