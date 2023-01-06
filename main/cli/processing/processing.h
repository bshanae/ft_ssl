#ifndef PROCESSING
#define PROCESSING

#include <stdint.h>

int typedef (*command_processor)(char **argv);

int process_md5_command(char **argv);
int process_sha256_command(char **argv);
int process_base64_command(char **argv);
int process_des_ecb_command(char **argv);
int process_des_cbc_command(char **argv);

// HELPERS

typedef void (*hash_function)(const char *string, uint8_t *hash);
int process_hash_command(char **argv, hash_function function, int hash_size, const char *algo_name);

#endif