#include <stdio.h>
#include "cli/task_resolution.h"
#include "cli/task_execution.h"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("usage: ft_ssl command [flags] [file/string]");
		return 0;
	}

	int argi = 1;

	struct task task;
	if (resolve_task(&task, &argi, argv) != 0)
		return 1;

	execute_task(&task, &argi, argv);
	return 0;
}