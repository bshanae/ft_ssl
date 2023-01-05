#include "processing.h"

#include <string.h>
#include "libft_standart.h"
#include "libft_ft_printf.h"
#include "tools/io_tools.h"
#include "tools/memory_tools.h"
#include "algo/tools/salt/salt.h"
#include "algo/des/des.h"

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

static int resolve_key_and_iv(struct options *options)
{
	if (options->has_key && options->has_iv)
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

int process_des_command(char **argv)
{
	int argi = 2;

	struct options options;
	if (resolve_options(&options, &argi, argv) != 0)
		return 1;

	if (resolve_key_and_iv(&options) != 0)
		return 1;

	ft_printf("key=%lx\n", options.key);

//	des_encrypt()

	return 0;
}