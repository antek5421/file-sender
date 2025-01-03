#ifndef HASH_H
#define HASH_H

#define BUFFER_SIZE 1024 * 1024
#define SHA256_DIGEST_LENGTH 32
#define AES_KEY_SIZE 16   // 128-bit AES key
#define AES_BLOCK_SIZE 16 // AES block size

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate_file_hash(const char *filePath, unsigned char *hashOutput);
bool compare_hashes(const unsigned char *hash1, const unsigned char *hash2,
                    size_t length);
int aes_encrypt_buffer(unsigned char *inputBuffer, int inputLength,
                       unsigned char *key, unsigned char *iv,
                       unsigned char *outputBuffer);

int aes_decrypt_buffer(unsigned char *encryptedBuffer, int encryptedLength,
                       unsigned char *key, unsigned char *iv,
                       unsigned char *decryptedBuffer);
#endif