#include "processing.h"

#include <string.h>
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

static int resolve_flags__(unsigned *flags, int *argi, char **argv)
{
	*flags = 0u;

	for (int i = *argi; argv[*argi] != NULL; i++)
	{
		if (argv[i][0] != '-')
			break;

		switch (argv[i][1])
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
				ft_printf("%fd_out" "Unknown flag: %s\n", STDERR_FILENO, argv[i]);
				return 1;
		}

		(*argi)++;
	}

	return 0;
}

void resolve_output_options__(struct output_options *options, unsigned flags, char allow_reverse, int allow_print_command)
{
	options->f_print = HAS_BIT(flags, F_PRINT);
	options->f_quiet = HAS_BIT(flags, F_QUIET);
	options->f_reverse = HAS_BIT(flags, F_REVERSE) && allow_reverse;
	options->print_command = allow_print_command && !options->f_quiet && !options->f_reverse;
	options->print_prefix = !options->f_reverse && (options->f_print || !options->f_quiet);
	options->print_postfix = options->f_reverse && !options->f_quiet;
}

void print_command__(const char *algo_name)
{
	char *command_name = ft_strdup(algo_name);
	for (int i = 0; i < ft_strlen(command_name); i++)
		to_upper(command_name + i);

	ft_printf("%s ", command_name);

	free(command_name);
}

void print_hash__(hash_function function, int hash_size, const char *input)
{
	uint8_t hash[256];
	function(input, hash);
	print_hex(hash, hash_size);
}

void process_stdin__(unsigned flags, hash_function function, int hash_size, const char *algo_name)
{
	// resolve input

	char *std_input = NULL;
	read_from_descriptor(&std_input, STDIN_FILENO);

	if (ft_strlen(std_input) == 0)
	{
		free(std_input);
		return;
	}

	// print

	struct output_options opts;
	resolve_output_options__(&opts, flags, 0, 0);

	if (opts.print_command)
		print_command__(algo_name);

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

	print_hash__(function, hash_size, std_input);

	ft_printf("\n");

	// cleanup

	free(std_input);
}

void process_string__(unsigned flags, hash_function function, int hash_size, const char *algo_name, int *argi, char **argv)
{
	// resolve input

	if (!HAS_BIT(flags, F_STRING))
		return;
	if (argv[*argi] == NULL)
	{
		ft_printf("%fd_out" "Expected a string input.\n", STDERR_FILENO);
		return;
	}

	char *string = argv[*argi];
	(*argi)++;

	// print

	struct output_options opts;
	resolve_output_options__(&opts, flags, 1, 1);

	if (opts.print_command)
		print_command__(algo_name);

	if (opts.print_prefix)
		ft_printf("(\"%s\") = ", string);

	print_hash__(function, hash_size, string);

	if (opts.print_postfix)
		ft_printf(" \"%s\"", string);

	ft_printf("\n");
}

void process_file__(unsigned flags, hash_function function, int hash_size, const char *algo_name, int *argi, char **argv)
{
	// resolve input

	if (argv[*argi] == NULL)
		return;

	char *filename = argv[*argi];
	(*argi)++;

	char *file = NULL;
	if (read_from_file(&file, filename) != 0)
	{
		ft_printf("%fd_out" "Can't read file '%s'\n", STDERR_FILENO, filename);
		return;
	}

	// print

	struct output_options opts;
	resolve_output_options__(&opts, flags, 1, 1);

	if (opts.print_command)
		print_command__(algo_name);

	if (opts.print_prefix)
		ft_printf("(%s) = ", filename);

	print_hash__(function, hash_size, file);

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
	if (resolve_flags__(&flags, &argi, argv) != 0)
		return 1;

	if (HAS_BIT(flags, F_PRINT) || (!HAS_BIT(flags, F_STRING) && argv[argi] == NULL))
		process_stdin__(flags, function, hash_size, algo_name);

	process_string__(flags, function, hash_size, algo_name, &argi, argv);
	process_file__(flags, function, hash_size, algo_name, &argi, argv);

	return 0;
}