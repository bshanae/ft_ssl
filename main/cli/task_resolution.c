#include "task_resolution.h"

#include "libft_standart.h"
#include "libft_ft_printf.h"

#pragma region private

int resolve_command(unsigned *command, int *argi, char **argv)
{
	const char *command_str = argv[*argi];

	if (ft_strcmp(command_str, "md5") == 0)
	{
		*command = TASK_COMMAND_MD5;
	}
	else if (ft_strcmp(command_str, "sha256") == 0)
	{
		*command = TASK_COMMAND_SHA256;
	}
	else
	{
		ft_printf("%fd_out" "[ft_ssl] Unknown command: %s\n", STDERR_FILENO, command_str);
		return 1;
	}

	(*argi)++;
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
