#include "cli.h"

#include <stdlib.h>
#include <unistd.h>
#include "libft_standart.h"
#include "libft_ft_printf.h"
#include "tools/io_tools.h"
#include "tools/bitflags_tools.h"

#pragma region private

#define SOURCE_STDIN 1
#define SOURCE_STR 2
#define SOURCE_FILE 3

void execute_task_with_target(struct task *task, const char *target, unsigned source)
{
	const int f_print = HAS_BIT(task->flags, TASK_FLAG_PRINT);
	const int f_quiet = HAS_BIT(task->flags, TASK_FLAG_QUIET);
	const int f_reverse = HAS_BIT(task->flags, TASK_FLAG_REVERSE) && source != SOURCE_STDIN;

	const int print_command = source != SOURCE_STDIN  && !f_reverse;
	const int print_prefix = !f_reverse && (!f_quiet || f_print);
	const int print_postfix = f_reverse && !f_quiet;

	if (print_command)
	{
		char *command_name = ft_strdup(task->command->name);
		for (int i = 0; i < ft_strlen(command_name); i++)
			to_upper(command_name + i);

		ft_printf("%s ", command_name);

		free(command_name);
	}

	if (print_prefix)
	{
		switch (source)
		{
			case SOURCE_STDIN:
				if (f_print)
				{
					if (!f_quiet)
					{
						char *trimmed_target = ft_strtrim(target);
						ft_printf("(\"%s\")= ", trimmed_target);
						free(trimmed_target);
					}
					else
					{
						ft_printf(target);
					}
				}
				else
					ft_printf("(stdin)= ");
				break;

			case SOURCE_STR:
				ft_printf("(\"%s\") = ", target);
				break;

			case SOURCE_FILE:
				ft_printf("(file) = ");
				break;

			default:
				ft_printf("%fd_out" "Unknown source.\n", STDERR_FILENO);
				break;
		}
	}

	uint8_t hash[256];
	task->command->function(target, hash);
	print_hex(hash, task->command->hash_size);

	if (print_postfix)
	{
		switch (source)
		{
			case SOURCE_STR:
				ft_printf(" \"%s\"", target);
				break;

			case SOURCE_FILE:
				ft_printf(" file");
				break;

			default:
				ft_printf("%fd_out" "Unknown source.\n", STDERR_FILENO);
				break;
		}
	}

	ft_printf("\n");
}

void process_stdin(struct task *task)
{
	char *std_input = NULL;
	read_from_descriptor(&std_input, STDIN_FILENO);

	if (ft_strlen(std_input) > 0)
		execute_task_with_target(task, std_input, SOURCE_STDIN);
	free(std_input);
}

void process_string(struct task *task, int *argi, char **argv)
{
	if (!HAS_BIT(task->flags, TASK_FLAG_STRING))
		return;
	if (argv[*argi] == NULL)
	{
		ft_printf("%fd_out" "Expected a string input.\n", STDERR_FILENO);
		return;
	}

	execute_task_with_target(task, argv[*argi], SOURCE_STR);
	(*argi)++;
}

void process_file(struct task *task, int *argi, char **argv)
{
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

	execute_task_with_target(task, file, SOURCE_FILE);
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