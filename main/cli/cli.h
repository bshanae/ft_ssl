#ifndef CLI
#define CLI

#define F_PRINT (1u << 0)
#define F_QUIET (1u << 1)
#define F_REVERSE (1u << 2)
#define F_STRING (1u << 3)

struct task
{
	struct command_descriptor *command;
	unsigned flags;
};

int process_arguments(int argc, char **argv);

#endif