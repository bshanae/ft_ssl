#include "commands.h"

#include "algo/md5/md5_interface.h"
#include "algo/sha256/sha256_interface.h"

struct command_descriptor command_descriptors[] =
{
	{ "md5", md5_string, 16 },
	{ "sha256", sha256_string, 32 }
};

unsigned command_descriptors_count = sizeof(command_descriptors) / sizeof(struct command_descriptor);