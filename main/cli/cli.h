#ifndef CLI
#define CLI

#include "commands.h"

#define TASK_FLAG_PRINT (1u << 0)
#define TASK_FLAG_QUIET (1u << 1)
#define TASK_FLAG_REVERSE (1u << 2)
#define TASK_FLAG_STRING (1u << 3)

struct task
{
	struct command_descriptor *command;
	unsigned flags;
};

int resolve_task(struct task *tasks, int *argi, char **argv);
void execute_task(struct task *task, int *argi, char **argv);

#endif