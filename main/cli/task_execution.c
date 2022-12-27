#include "cli.h"

#include <stdlib.h>
#include <unistd.h>
#include "libft_standart.h"
#include "libft_ft_printf.h"
#include "tools/io_tools.h"
#include "tools/bitflags_tools.h"

#pragma region private

struct output_options
{
	char f_print;
	char f_quiet;
	char f_reverse;
	char print_command;
	char print_prefix;
	char print_postfix;
};

void resolve_output_options(struct output_options *options, struct task *task, char allow_reverse, char allow_print_command)
{
	options->f_print = HAS_BIT(task->flags, TASK_FLAG_PRINT);
	options->f_quiet = HAS_BIT(task->flags, TASK_FLAG_QUIET);
	options->f_reverse = HAS_BIT(task->flags, TASK_FLAG_REVERSE) && allow_reverse;
	options->print_command = allow_print_command && !options->f_quiet && !options->f_reverse;
	options->print_prefix = !options->f_reverse && (options->f_print || !options->f_quiet);
	options->print_postfix = options->f_reverse && !options->f_quiet;
}

void print_command(struct task *task)
{
	char *command_name = ft_strdup(task->command->name);
	for (int i = 0; i < ft_strlen(command_name); i++)
		to_upper(command_name + i);

	ft_printf("%s ", command_name);

	free(command_name);
}

void print_hash(struct task *task, const char *target)
{
	uint8_t hash[256];
	task->command->function(target, hash);
	print_hex(hash, task->command->hash_size);
}

void process_stdin(struct task *task)
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
	resolve_output_options(&opts, task, 0, 0);

	if (opts.print_command)
		print_command(task);

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

	print_hash(task, std_input);

	ft_printf("\n");

	// cleanup

	free(std_input);
}

void process_string(struct task *task, int *argi, char **argv)
{
	// resolve input

	if (!HAS_BIT(task->flags, TASK_FLAG_STRING))
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
	resolve_output_options(&opts, task, 1, 1);

	if (opts.print_command)
		print_command(task);

	if (opts.print_prefix)
		ft_printf("(\"%s\") = ", string);

	print_hash(task, string);

	if (opts.print_postfix)
		ft_printf(" \"%s\"", string);

	ft_printf("\n");
}

void process_file(struct task *task, int *argi, char **argv)
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
	resolve_output_options(&opts, task, 1, 1);

	if (opts.print_command)
		print_command(task);

	if (opts.print_prefix)
		ft_printf("(%s) = ", filename);

	print_hash(task, file);

	if (opts.print_postfix)
		ft_printf(" %s", filename);

	ft_printf("\n");

	// cleanup

	free(file);
}

#pragma endregion

void execute_task(struct task *task, int *argi, char **argv)
{
	if (HAS_BIT(task->flags, TASK_FLAG_PRINT) || (!HAS_BIT(task->flags, TASK_FLAG_STRING) && argv[*argi] == NULL))
		process_stdin(task);

	process_string(task, argi, argv);
	process_file(task, argi, argv);
}