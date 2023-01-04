#include "processing.h"

#include "algo/sha256/sha256_interface.h"

int process_sha256_command(char **argv)
{
	return process_hash_command(argv, sha256_string, 32, "sha256");
}