#ifndef COMMANDS
#define COMMANDS

#include <stdint.h>

struct command_descriptor
{
	const char *name;
	void (*function)(const char *string, uint8_t *hash);
	unsigned hash_size;
};

extern struct command_descriptor command_descriptors[];
extern unsigned command_descriptors_count;

#endif