#include "../include/hash.h"

void calculate_file_hash(const char *filename, unsigned char *hashOutput) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    perror("Error creating context");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
    perror("Error initializing digest");
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  unsigned char buffer[BUFFER_SIZE];
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    if (EVP_DigestUpdate(mdctx, buffer, bytesRead) != 1) {
      perror("Error updating digest");
      EVP_MD_CTX_free(mdctx);
      fclose(file);
      exit(EXIT_FAILURE);
    }
  }

  if (ferror(file)) {
    perror("Error reading file");
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  unsigned int hashLength;
  if (EVP_DigestFinal_ex(mdctx, hashOutput, &hashLength) != 1) {
    perror("Error finalizing digest");
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  EVP_MD_CTX_free(mdctx);
  fclose(file);
}

bool compare_hashes(const unsigned char *hash1, const unsigned char *hash2,
                    size_t length) {
  if (memcmp(hash1, hash2, length) == 0)
    return true;
  else
    return false;
}

int aes_encrypt_buffer(unsigned char *inputBuffer, int inputLength,
                       unsigned char *key, unsigned char *iv,
                       unsigned char *outputBuffer) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    perror("EVP_CIPHER_CTX_new failed");
    return -1;
  }

  int numBytesEncrypted = 0;
  int outLen = 0;

  // Inicjalizacja szyfrowania
  if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
    perror("EVP_EncryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return -1;
  }

  // Szyfrowanie danych w blokach
  for (int i = 0; i < inputLength; i += AES_BLOCK_SIZE) {
    int blockSize = (inputLength - i) >= AES_BLOCK_SIZE ? AES_BLOCK_SIZE
                                                        : (inputLength - i);

    if (EVP_EncryptUpdate(ctx, outputBuffer + numBytesEncrypted, &outLen,
                          inputBuffer + i, blockSize) != 1) {
      perror("EVP_EncryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return -1;
    }

    numBytesEncrypted += outLen;
  }

  // Zakończenie szyfrowania
  if (EVP_EncryptFinal_ex(ctx, outputBuffer + numBytesEncrypted, &outLen) !=
      1) {
    perror("EVP_EncryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return -1;
  }

  numBytesEncrypted += outLen;
  EVP_CIPHER_CTX_free(ctx);

  return numBytesEncrypted;
}

int aes_decrypt_buffer(unsigned char *encryptedBuffer, int encryptedLength,
                       unsigned char *key, unsigned char *iv,
                       unsigned char *decryptedBuffer) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    perror("EVP_CIPHER_CTX_new failed");
    return -1;
  }

  int numBytesDecrypted = 0;
  int outLen = 0;

  // Inicjalizacja deszyfrowania
  if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
    perror("EVP_DecryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return -1;
  }

  // Deszyfrowanie danych w blokach
  for (int i = 0; i < encryptedLength; i += AES_BLOCK_SIZE) {
    int blockSize = (encryptedLength - i) >= AES_BLOCK_SIZE
                        ? AES_BLOCK_SIZE
                        : (encryptedLength - i);

    if (EVP_DecryptUpdate(ctx, decryptedBuffer + numBytesDecrypted, &outLen,
                          encryptedBuffer + i, blockSize) != 1) {
      perror("EVP_DecryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return -1;
    }

    numBytesDecrypted += outLen;
  }

  // Zakończenie deszyfrowania
  if (EVP_DecryptFinal_ex(ctx, decryptedBuffer + numBytesDecrypted, &outLen) !=
      1) {
    perror("EVP_DecryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return -1;
  }

  numBytesDecrypted += outLen;
  EVP_CIPHER_CTX_free(ctx);

  return numBytesDecrypted;
}