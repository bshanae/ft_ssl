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

int read_from_file(const char *path, char **data)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1)
		return 1;

	int result = read_from_descriptor(data, fd);
	close(fd);

	return result;
}

int write_to_file(const char *path, const char *data, const size_t size)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return 1;

	write(fd, data, size);
	close(fd);

	return 0;
}

void print_hex(const void *data, unsigned size)
{
	const uint8_t *data8 = (const uint8_t *)data;
	for (unsigned i = 0; i < size; ++i)
		ft_printf("%02x", data8[i]);
}

void print_error(const char *str)
{
	ft_printf("%fd_out" "%s\n", STDERR_FILENO, str);
}