#include "processing.h"

#include <unistd.h>
#include "libft_standart.h"
#include "libft_ft_printf.h"
#include "tools/io_tools.h"
#include "tools/bitflags_tools.h"

#define F_PRINT (1u << 0)
#define F_QUIET (1u << 1)
#define F_REVERSE (1u << 2)
#define F_STRING (1u << 3)

struct output_options
{
	int f_print;
	int f_quiet;
	int f_reverse;
	int print_command;
	int print_prefix;
	int print_postfix;
};

static int resolve_flags(unsigned *flags, int *argi, char **argv)
{
	*flags = 0u;

	for (; argv[*argi] != NULL; (*argi)++)
	{
		if (argv[*argi][0] != '-')
			break;

		switch (argv[*argi][1])
		{
			case 'p':
				*flags |= F_PRINT;
				break;

			case 'q':
				*flags |= F_QUIET;
				break;

			case 'r':
				*flags |= F_REVERSE;
				break;

			case 's':
				*flags |= F_STRING;
				break;

			default:
				print_error("Unknown flag.");
				return 1;
		}
	}

	return 0;
}

void resolve_output_options(struct output_options *options, unsigned flags, char allow_reverse, int allow_print_command)
{
	options->f_print = HAS_BIT(flags, F_PRINT);
	options->f_quiet = HAS_BIT(flags, F_QUIET);
	options->f_reverse = HAS_BIT(flags, F_REVERSE) && allow_reverse;
	options->print_command = allow_print_command && !options->f_quiet && !options->f_reverse;
	options->print_prefix = !options->f_reverse && (options->f_print || !options->f_quiet);
	options->print_postfix = options->f_reverse && !options->f_quiet;
}

void print_command(const char *algo_name)
{
	char *command_name = ft_strdup(algo_name);
	for (int i = 0; i < ft_strlen(command_name); i++)
		to_upper(command_name + i);

	ft_printf("%s ", command_name);

	free(command_name);
}

void print_hash(hash_function function, int hash_size, const char *input)
{
	uint8_t hash[256];
	function(input, hash);
	print_hex(hash, hash_size);
}

void process_stdin(unsigned flags, hash_function function, int hash_size, const char *algo_name)
{
	// resolve input

	char *std_input = NULL;
	size_t std_input_size;
	read_from_descriptor(STDIN_FILENO, &std_input, &std_input_size);

	if (std_input_size == 0)
	{
		free(std_input);
		return;
	}

	// print

	struct output_options opts;
	resolve_output_options(&opts, flags, 0, 0);

	if (opts.print_command)
		print_command(algo_name);

	if (opts.print_prefix)
	{
		if (opts.f_print)
		{
			if (!opts.f_quiet)
			{
				char *trimmed_target = ft_strtrim(std_input);
				ft_printf("(\"%s\")= ", trimmed_target);
				free(trimmed_target);
			}
			else
			{
				ft_printf(std_input);
			}
		}
		else
		{
			ft_printf("(stdin)= ");
		}
	}

	print_hash(function, hash_size, std_input);

	ft_printf("\n");

	// cleanup

	free(std_input);
}

void process_string(unsigned flags, hash_function function, int hash_size, const char *algo_name, int *argi, char **argv)
{
	// resolve input

	if (!HAS_BIT(flags, F_STRING))
		return;
	if (argv[*argi] == NULL)
	{
		print_error("Expected a string input.");
		return;
	}

	char *string = argv[*argi];
	(*argi)++;

	// print

	struct output_options opts;
	resolve_output_options(&opts, flags, 1, 1);

	if (opts.print_command)
		print_command(algo_name);

	if (opts.print_prefix)
		ft_printf("(\"%s\") = ", string);

	print_hash(function, hash_size, string);

	if (opts.print_postfix)
		ft_printf(" \"%s\"", string);

	ft_printf("\n");
}

void process_file(unsigned flags, hash_function function, int hash_size, const char *algo_name, int *argi, char **argv)
{
	// resolve input

	if (argv[*argi] == NULL)
		return;

	char *filename = argv[*argi];
	(*argi)++;

	char *file = NULL;
	size_t file_size;
	if (read_from_file(filename, &file, &file_size) != 0)
	{
		print_error("Can't read file.");
		return;
	}

	// print

	struct output_options opts;
	resolve_output_options(&opts, flags, 1, 1);

	if (opts.print_command)
		print_command(algo_name);

	if (opts.print_prefix)
		ft_printf("(%s) = ", filename);

	print_hash(function, hash_size, file);

	if (opts.print_postfix)
		ft_printf(" %s", filename);

	ft_printf("\n");

	// cleanup

	free(file);
}

int process_hash_command(char **argv, hash_function function, int hash_size, const char *algo_name)
{
	int argi = 2;

	unsigned flags;
	if (resolve_flags(&flags, &argi, argv) != 0)
		return 1;

	if (HAS_BIT(flags, F_PRINT) || (!HAS_BIT(flags, F_STRING) && argv[argi] == NULL))
		process_stdin(flags, function, hash_size, algo_name);

	process_string(flags, function, hash_size, algo_name, &argi, argv);
	process_file(flags, function, hash_size, algo_name, &argi, argv);

	return 0;
}