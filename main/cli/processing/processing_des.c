#include "processing.h"

#include "libft_standart.h"
#include "tools/io_tools.h"
#include "tools/math_tools.h"
#include "tools/memory_tools.h"
#include "algo/tools/salt.h"
#include "algo/base64/base64.h"
#include "algo/des/des_interface.h"

#define M_ENCRYPT 0
#define M_DECRYPT 1

struct options
{
	int apply_base64;
	int mode;
	char *input_file;
	char *output_file;
	char *password;
	int has_salt;
	uint64_t salt;
	int has_key;
	uint64_t key;
	int has_iv;
	uint64_t iv;
};

typedef void (* des_encrypt_functor)(const void *plaintext, size_t plaintext_size, void *ciphertext, const uint64_t *key, const uint64_t *iv);

typedef int (* des_decrypt_functor)(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key, const uint64_t *iv);

uint64_t parse_h64(const char *str)
{
	char final_str[17];

	int i = 0;
	for (; i < 16 && str[i] != 0; i++)
		final_str[i] = str[i];
	for (; i < 16; i++)
		final_str[i] = '0';
	final_str[16] = '\0';

	return ft_atol_base(final_str, 16);
}

int resolve_options(struct options *options, int *argi, char **argv)
{
	ft_bzero(options, sizeof(struct options));

	for (; argv[*argi] != NULL; (*argi)++)
	{
		if (argv[*argi][0] != '-')
			break;

		switch (argv[*argi][1])
		{
			case 'a':
				options->apply_base64 = 1;
				break;

			case 'd':
				options->mode = M_DECRYPT;
				break;

			case 'e':
				options->mode = M_ENCRYPT;
				break;

			case 'i':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected an input file.");
					return 1;
				}

				options->input_file = argv[++(*argi)];
				break;

			case 'o':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected an output file.");
					return 1;
				}

				options->output_file = argv[++(*argi)];
				break;

			case 'p':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected a password.");
					return 1;
				}

				options->password = argv[++(*argi)];
				break;

			case 's':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected a salt.");
					return 1;
				}

				options->has_salt = 1;
				options->salt = parse_h64(argv[++(*argi)]);
				break;

			case 'k':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected a key.");
					return 1;
				}

				options->has_key = 1;
				options->key = parse_h64(argv[++(*argi)]);
				break;


			case 'v':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected an IV.");
					return 1;
				}

				options->has_iv = 1;
				options->iv = parse_h64(argv[++(*argi)]);
				break;

			default:
				print_error("Unknown flag.");
				return 1;
		}
	}

	return 0;
}

static int resolve_input(const struct options *options, char **input)
{
	if (options->input_file != NULL)
	{
		*input = NULL;
		if (read_from_file(options->input_file, input) != 0)
		{
			print_error("Failed to read from file!");
			free(*input);
			return 1;
		}

		return 0;
	}
	else
	{
		char *std_input = NULL;
		read_from_descriptor(&std_input, STDIN_FILENO);

		if (ft_strlen(std_input) == 0)
		{
			free(std_input);
			return 1;
		}

		*input = std_input;
		return 0;
	}
}

static int resolve_key_and_iv(struct options *options, const int need_iv, char **input, size_t *input_size)
{
	if (options->has_key && (options->has_iv || !need_iv))
		return 0;

	if (options->password == NULL)
	{
		static const char *encryption_message = "enter des encryption password: ";
		static const char *decryption_message = "enter des decryption password: ";

		options->password = getpass(options->mode == M_ENCRYPT ? encryption_message : decryption_message);
		if (options->password == NULL)
		{
			print_error("Can't read password.");
			return 1;
		}
	}

	if (!options->has_salt)
	{
		if (options->mode == M_ENCRYPT)
		{
			options->has_salt = 1;
			options->salt = generate_salt();
		}
		else
		{
			if (ft_strncmp(*input, "Salted__", 8) != 0)
			{
				print_error("Salt is not found.");
				return 1;
			}
			if (*input_size <= 16)
			{
				print_error("Invalid ciphertext.");
				return 1;
			}

			options->has_salt = 1;
			options->salt = to_bigendian_64(*(uint64_t *)(*input + 8));

			const size_t temp_size = *input_size - 16;
			char *temp = malloc(temp_size);
			ft_strcpy(temp, *input + 16);

			free(*input);
			*input = temp;
			*input_size = temp_size;
		}
	}

	uint32_t hash[4];
	des_generate_key(options->password, options->salt, (uint8_t *)hash);

	if (!options->has_key)
	{
		options->has_key = 1;
		options->key = ((uint64_t) SWAP_32(hash[0]) << 32) | SWAP_32(hash[1]);
	}
	if (!options->has_iv)
	{
		options->has_iv = 1;
		options->iv = ((uint64_t) SWAP_32(hash[2]) << 32) | SWAP_32(hash[3]);
	}

	return 0;
}

static void write_output(const struct options *options, const char *output, const size_t output_size, int newline)
{
	if (options->output_file != NULL)
	{
		if (write_to_file(options->output_file, output, output_size) != 0)
			print_error("Failed to write to file!");
	}
	else
	{
		write(STDOUT_FILENO, output, output_size);
		if (newline)
			write(STDOUT_FILENO, "\n", 1);
	}
}

static int process_des_command(char **argv, des_encrypt_functor encryptor, des_decrypt_functor decryptor, const int use_iv)
{
	int argi = 2;

	// resolve options

	struct options options;
	if (resolve_options(&options, &argi, argv) != 0)
		return 1;

	// resolve input

	char *input;
	if (resolve_input(&options, &input) != 0)
		return 1;

	size_t input_size = ft_strlen(input);

	// resolve key and iv

	if (resolve_key_and_iv(&options, use_iv, &input, &input_size) != 0)
	{
		free(input);
		return 1;
	}

	// decode with base64

	if (options.mode == M_DECRYPT && options.apply_base64)
	{
		size_t temp_size = base64_max_decoded_size(input_size);
		char *temp = malloc(temp_size + 1);

		base64_decode(input, input_size, temp, &temp_size);
		temp[temp_size] = '\0';

		free(input);
		input = temp;
		input_size = temp_size;
	}

	// encode/decode

	size_t output_size = (options.mode == M_ENCRYPT ? des_encrypted_size(input_size) : input_size);
	char *output = malloc(output_size + 1);

	if (options.mode == M_ENCRYPT)
		encryptor(input, input_size, output, &options.key, &options.iv);
	else
	{
		if (decryptor(output, &output_size, input, ROUND_DOWN(input_size, DES_BLOCK_SIZE), &options.key, &options.iv) != 0)
		{
			free(input);
			free(output);
			return 1;
		}
	}

	output[output_size] = '\0';

	// encode with base64

	if (options.mode == M_ENCRYPT && options.apply_base64)
	{
		const size_t temp_size = base64_encoded_size(output_size);
		char *temp = malloc(temp_size + 1);
		temp[temp_size] = '\0';

		base64_encode(output, output_size, temp, 1);

		free(output);
		output = temp;
		output_size = temp_size;
	}

	// write

	if (options.mode == M_ENCRYPT && options.password != NULL)
	{
		char header[16];
		ft_strcpy(header, "Salted__");
		ft_memcpy(header + 8, &options.salt, 8);

		write_output(&options, header, 16, 0);
	}

	write_output(&options, output, output_size, 1);

	// exit

	free(input);
	free(output);

	return 0;
}

void des_encrypt_ecb_wrapper(const void *plaintext, size_t plaintext_size, void *ciphertext, const uint64_t *key, const uint64_t *_)
{
	des_encrypt_ecb(plaintext, plaintext_size, ciphertext, key);
}

int des_decrypt_ecb_wrapper(void *plaintext, size_t *plaintext_size, const void *ciphertext, size_t ciphertext_size, const uint64_t *key, const uint64_t *_)
{
	return des_decrypt_ecb(plaintext, plaintext_size, ciphertext, ciphertext_size, key);
}

int process_des_ecb_command(char **argv)
{
	return process_des_command(argv, des_encrypt_ecb_wrapper, des_decrypt_ecb_wrapper, 0);
}

int process_des_cbc_command(char **argv)
{
	return process_des_command(argv, des_encrypt_cbc, des_decrypt_cbc, 1);
}