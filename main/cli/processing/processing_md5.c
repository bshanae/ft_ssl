#include "processing.h"

#include "algo/md5/md5_interface.h"

int process_md5_command(char **argv)
{
	return process_hash_command(argv, md5_string, 16, "md5");
}