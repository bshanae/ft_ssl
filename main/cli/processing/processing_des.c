#include "processing.h"

#include "libft_standart.h"
#include "tools/io_tools.h"
#include "tools/memory_tools.h"
#include "algo/tools/salt.h"
#include "algo/base64/base64.h"
#include "algo/des/des_interface.h"

#define M_ENCRYPT 0
#define M_DECRYPT 1

struct options
{
	int encode_with_base64;
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
				options->encode_with_base64 = 1;
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

static int resolve_key_and_iv(struct options *options, const int need_iv)
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

	if (!options->has_salt && options->mode == M_ENCRYPT)
	{
		options->has_salt = 1;
		options->salt = generate_salt();
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

static int resolve_plaintext(const struct options *options, char **plaintext)
{
	if (options->input_file != NULL)
	{
		*plaintext = NULL;
		if (read_from_file(options->input_file, plaintext) != 0)
		{
			print_error("Failed to read from file!");
			free(*plaintext);
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

		*plaintext = std_input;
		return 0;
	}
}

void encode_ciphertext_with_base64(const char *ciphertext, const size_t ciphertext_size, char **ciphertext_b64, size_t *ciphertext_b64_size)
{
	*ciphertext_b64_size = base64_encoded_size(ciphertext_size);
	*ciphertext_b64 = malloc(*ciphertext_b64_size + 1);

	base64_encode(*ciphertext_b64, ciphertext, ciphertext_size, 1);
	(*ciphertext_b64)[*ciphertext_b64_size] = 0;
}

static void write_ciphertext(const struct options *options, const char *ciphertext, const size_t ciphertext_size)
{
	if (options->output_file != NULL)
	{
		if (write_to_file(options->output_file, ciphertext, ciphertext_size) != 0)
			print_error("Failed to write to file!");
	}
	else
	{
		write(STDOUT_FILENO, ciphertext, ciphertext_size);
		write(STDOUT_FILENO, "\n", 1);
	}
}

typedef void (* des_function)(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key);
typedef void (* des_function_with_iv)(const void *plaintext, void *ciphertext, size_t size, const uint64_t *key, const uint64_t *iv);

static int process_des_command(char **argv, void *des, const int use_iv)
{
	int argi = 2;

	// resolve options

	struct options options;
	if (resolve_options(&options, &argi, argv) != 0)
		return 1;

	// resolve key and iv

	if (resolve_key_and_iv(&options, use_iv) != 0)
		return 1;

	// resolve plaintext

	char *plaintext;
	if (resolve_plaintext(&options, &plaintext) != 0)
		return 1;

	// compute plaintext and ciphertext sizes

	const size_t plaintext_size = ft_strlen(plaintext);
	const size_t ciphertext_size = des_encrypted_size(plaintext_size);

	// generate ciphertext

	char *ciphertext = malloc(ciphertext_size + 1);
	ciphertext[ciphertext_size] = 0;
	if (use_iv)
		((des_function_with_iv)des)(plaintext, ciphertext, ft_strlen(plaintext), &options.key, &options.iv);
	else
		((des_function)des)(plaintext, ciphertext, ft_strlen(plaintext), &options.key);

	// encode ciphertext with base64

	char *ciphertext_b64 = NULL;
	size_t ciphertext_b64_size = 0;
	if (options.encode_with_base64)
		encode_ciphertext_with_base64(ciphertext, ciphertext_size, &ciphertext_b64, &ciphertext_b64_size);

	// output ciphertext

	if (ciphertext_b64 != NULL)
		write_ciphertext(&options, ciphertext_b64, ciphertext_b64_size);
	else
		write_ciphertext(&options, ciphertext, ciphertext_size);

	// exit

	free(plaintext);
	free(ciphertext);
	free(ciphertext_b64);

	return 0;
}

int process_des_ecb_command(char **argv)
{
	return process_des_command(argv, des_ecb, 0);
}

int process_des_cbc_command(char **argv)
{
	return process_des_command(argv, des_cbc, 1);
}