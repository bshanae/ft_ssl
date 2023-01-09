#include "cli.h"

#include <string.h>
#include "libft_ft_printf.h"
#include "libft_standart.h"
#include "tools/io_tools.h"
#include "processing/processing.h"

static void print_usage()
{
	ft_printf(
		"Standard commands:\n\n"
		"Message Digest commands:\n"
		"md5\n"
		"sha256\n\n"
		"Cipher commands:\n"
		"base64\n"
		"des\n"
		"des-ecb\n"
		"des-cbc\n"
	);
}

static command_processor find_command_processor(char *command)
{
	if (ft_strcmp(command, "md5") == 0)
		return process_md5_command;
	if (ft_strcmp(command, "sha256") == 0)
		return process_sha256_command;
	if (ft_strcmp(command, "base64") == 0)
		return process_base64_command;
	if (ft_strcmp(command, "des") == 0)
		return process_des_cbc_command;
	if (ft_strcmp(command, "des-ecb") == 0)
		return process_des_ecb_command;
	if (ft_strcmp(command, "des-cbc") == 0)
		return process_des_cbc_command;

	print_error("Unknown command.");
	return NULL;
}

int process_arguments(int argc, char **argv)
{
	if (argc == 1)
	{
		print_usage();
		return 0;
	}

	command_processor processor = find_command_processor(argv[1]);
	if (processor == NULL)
	{
		print_usage();
		return 1;
	}

	return processor(argv);
}