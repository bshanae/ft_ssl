#include "processing.h"

#include <stdlib.h>
#include "libft_standart.h"
#include "tools/io_tools.h"
#include "algo/base64/base64.h"

#define M_ENCRYPT 0
#define M_DECRYPT 1

struct options
{
	int mode;
	char *input_file;
	char *output_file;
};

static int resolve_options(struct options *options, int *argi, char **argv)
{
	ft_bzero(options, sizeof(struct options));

	for (; argv[*argi] != NULL; (*argi)++)
	{
		if (argv[*argi][0] != '-')
			break;

		switch (argv[*argi][1])
		{
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

			default:
				print_error("Unknown flag.");
				return 1;
		}
	}

	return 0;
}

static int resolve_input(const struct options *options, char **input, size_t *input_size)
{
	if (options->input_file != NULL)
	{
		*input = NULL;
		if (read_from_file(options->input_file, input, input_size) != 0)
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
		size_t std_input_size;
		read_from_descriptor(STDIN_FILENO, &std_input, &std_input_size);

		if (std_input_size == 0)
		{
			free(std_input);
			return 1;
		}

		*input = std_input;
		*input_size = std_input_size;

		return 0;
	}
}

static void write_output(const struct options *options, const char *output, const size_t output_size)
{
	if (options->output_file != NULL)
	{
		if (write_to_file(options->output_file, output, output_size) != 0)
			print_error("Failed to write to file!");
	}
	else
	{
		write(STDOUT_FILENO, output, output_size);
		write(STDOUT_FILENO, "\n", 1);
	}
}

int process_base64_command(char **argv)
{
	int argi = 2;

	struct options options;
	if (resolve_options(&options, &argi, argv) != 0)
		return 1;

	char *input;
	size_t input_size;
	if (resolve_input(&options, &input, &input_size) != 0)
		return 1;

	char *output;
	size_t output_size;
	if (options.mode == M_ENCRYPT)
	{
		output_size = base64_encoded_size(input_size);
		output = malloc(output_size + 1);

		base64_encode(input, input_size, output, 1);
	}
	else
	{
		output_size = base64_max_decoded_size(input_size);
		output = malloc(output_size + 1);

		base64_decode(input, input_size, output, &output_size);
		output[output_size] = '\0';
	}

	write_output(&options, output, output_size);

	free(output);

	return 0;
}