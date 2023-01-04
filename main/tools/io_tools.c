#include "io_tools.h"

#include <unistd.h>
#include <stdlib.h>
#include <printf.h>
#include "libft_standart.h"
#include "libft_ft_printf.h"

#define INITIAL_SIZE 512
int read_from_descriptor(char **data, int fd)
{
	*data = NULL;

	char *buffer = malloc(INITIAL_SIZE);

	size_t total_size = INITIAL_SIZE;
	size_t available_size = INITIAL_SIZE - 1;
	size_t used_size = 0;

	while (1)
	{
		size_t read_size = read(fd, buffer + used_size, available_size);
		if (read_size == -1)
		{
			free(buffer);
			return 1;
		}
		if (read_size == 0)
			break;

		used_size += read_size;
		available_size -= read_size;

		if (buffer[used_size - 1] == '\0')
			break;
		if (buffer[used_size] == '\0')
			break;

		if (available_size == 0)
		{
			buffer = ft_realloc((void **)&buffer, total_size, total_size * 2);
			if (buffer == NULL)
				return 1;

			total_size *= 2;
			available_size = total_size - used_size - 1;
		}
	}

	buffer[used_size] = '\0';

	*data = buffer;
	return 0;
}

int read_from_file(char **data, const char *path)
{
	int fd = open(path, S_IREAD);
	if (fd == -1)
		return 1;

	int result = read_from_descriptor(data, fd);
	close(fd);

	return result;
}

void print_hex(uint8_t *data, unsigned size)
{
	for (unsigned i = 0; i < size; ++i)
		ft_printf("%02x", data[i]);
}

void print_error(const char *str)
{
	ft_printf("%fd_out" "%s\n", STDERR_FILENO, str);
}