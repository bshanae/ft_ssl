#include "cli.h"

#include <string.h>
#include "libft_ft_printf.h"
#include "libft_standart.h"
#include "processing/processing.h"

static command_processor find_command_processor(char *command)
{
	if (ft_strcmp(command, "md5") == 0)
		return process_md5_command;
	if (ft_strcmp(command, "sha256") == 0)
		return process_sha256_command;
//	if (ft_strcmp(command, "base64") == 0)
//		return process_base64_command;
	if (ft_strcmp(command, "des") == 0)
		return process_des_command;

	return NULL;
}

int process_arguments(int argc, char **argv)
{
	if (argc == 1)
	{
		ft_printf("usage: ft_ssl command [flags] [file/string]"); // TODO
		return 0;
	}

	command_processor processor = find_command_processor(argv[1]);
	if (processor == NULL)
	{
		ft_printf("usage: ft_ssl command [flags] [file/string]"); // TODO
		return 1;
	}

	processor(argv);
	return 0;
}