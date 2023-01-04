#include "processing.h"

#include <string.h>
#include <libc.h>
#include "libft_ft_printf.h"
#include "tools/io_tools.h"

struct options
{
	int f_base64;
	int f_decrypt;
	int f_encrypt;
	char *input_file;
	char *output_file;
	char *password;
	char *salt;
	uint64_t key;
	uint64_t iv;
};

int resolve_options(struct options *options, int *argi, char **argv)
{
	options->f_base64 = 0;
	options->f_decrypt = 0;
	options->f_encrypt = 0;
	options->input_file = 0;

	for (; argv[*argi] != NULL; (*argi)++)
	{
		if (argv[*argi][0] != '-')
			break;

		switch (argv[*argi][1])
		{
			case 'a':
				options->f_base64 = 1;
				break;

			case 'd':
				options->f_decrypt = 1;
				break;

			case 'e':
				options->f_encrypt = 1;
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

				options->salt = argv[++(*argi)];
				break;

			case 'k':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected a key.");
					return 1;
				}

				options->key = 0; // TODO
				break;


			case 'v':
				if (argv[*argi + 1] == NULL)
				{
					print_error("Expected an IV.");
					return 1;
				}

				options->iv = 0; // TODO
				break;

			default:
				print_error("Unknown flag:.");
				return 1;
		}
	}

	return 0;
}

int process_des_command(char **argv)
{
	int argi = 2;

	struct options options;
	if (resolve_options(&options, &argi, argv) != 0)
		return 1;

	return 0;
}