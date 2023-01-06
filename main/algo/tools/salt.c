#ifndef SALT
#define SALT

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

uint64_t generate_salt()
{
	const int fd = open("/dev/random", O_RDONLY);

	uint64_t salt;
	read(fd, &salt, 8);

	close(fd);
	return salt;
}

#endif