#include "cli.h"

#include "libft_standart.h"
#include "libft_ft_printf.h"

#pragma region private

int resolve_command(struct command_descriptor **command, int *argi, char **argv)
{
	const char *command_str = argv[*argi];
	(*argi)++;

	*command = NULL;
	for (int i = 0; i < command_descriptors_count; i++)
	{
		struct command_descriptor *test_command = command_descriptors + i;
		if (ft_strcmp(command_str, test_command->name) == 0)
			*command = test_command;
	}

	if (*command == NULL)
	{
		ft_printf("%fd_out" "[ft_ssl] Unknown command: %s\n", STDERR_FILENO, command_str);
		return 1;
	}

	return 0;
}

int resolve_flags(unsigned *flags, int *argi, char **argv)
{
	*flags = 0u;

	for (int i = *argi; argv[*argi] != NULL; i++)
	{
		if (argv[i][0] != '-')
			break;

		switch (argv[i][1])
		{
			case 'p':
				*flags |= TASK_FLAG_PRINT;
				break;

			case 'q':
				*flags |= TASK_FLAG_QUIET;
				break;

			case 'r':
				*flags |= TASK_FLAG_REVERSE;
				break;

			case 's':
				*flags |= TASK_FLAG_STRING;
				break;

			default:
				ft_printf("%fd_out" "Unknown flag: %s\n", STDERR_FILENO, argv[i]);
				return 1;
		}

		(*argi)++;
	}

	return 0;
}

#pragma endregion

int resolve_task(struct task *task, int *argi, char **argv)
{
	if (resolve_command(&task->command, argi, argv) != 0)
		return 1;
	if (resolve_flags(&task->flags, argi, argv) != 0)
		return 1;

	return 0;
}
