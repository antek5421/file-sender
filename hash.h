#ifndef HASH_H
#define HASH_H

#define BUFFER_SIZE 1024

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate_file_hash(const char *filePath, unsigned char *hashOutput);
bool compare_hashes(const unsigned char *hash1, const unsigned char *hash2,
                    size_t length);

#endif