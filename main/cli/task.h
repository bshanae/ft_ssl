#ifndef TASK
#define TASK

#define TASK_COMMAND_MD5 (1u << 1)
#define TASK_COMMAND_SHA256 (1u << 2)

#define TASK_FLAG_PRINT (1u << 0)
#define TASK_FLAG_QUIET (1u << 1)
#define TASK_FLAG_REVERSE (1u << 2)
#define TASK_FLAG_STRING (1u << 3)

struct task
{
	unsigned command;
	unsigned flags;
};

#endif