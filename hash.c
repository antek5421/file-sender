#include "hash.h"

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