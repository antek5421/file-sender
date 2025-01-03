#include "../include/client.h"

void client(const char *IP, const char *filePath) {
  int sockfd;
  struct sockaddr_in servaddr;
  char buffer[BUFFER_SIZE];
  size_t bytesRead;

  unsigned char hash[SHA256_DIGEST_LENGTH];
  calculate_file_hash(filePath, hash);

  unsigned char key[AES_KEY_SIZE];
  unsigned char iv[AES_BLOCK_SIZE];

  if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
    perror("Error generating AES key/IV");
    exit(EXIT_FAILURE);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(IP);
  servaddr.sin_port = htons(PORT);

  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
    perror("Connection to the server failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Connected to the server: %s\n", IP);

  printf("Sending file hash to server\n");
  if (send(sockfd, hash, SHA256_DIGEST_LENGTH, 0) == -1) {
    perror("Error sending hash");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Sending AES key and IV to the server\n");
  if (send(sockfd, key, AES_KEY_SIZE, 0) == -1 ||
      send(sockfd, iv, AES_BLOCK_SIZE, 0) == -1) {
    perror("Error sending key/IV");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(filePath, "rb");
  if (file == NULL) {
    perror("Error opening file");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Sending encrypted file: %s to server\n", filePath);

  while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
    unsigned char encryptedBuffer[BUFFER_SIZE];
    int encryptedLength = aes_encrypt_buffer((unsigned char *)buffer, bytesRead,
                                             key, iv, encryptedBuffer);

    if (encryptedLength < 0) {
      perror("Error encrypting data");
      fclose(file);
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    // Sending the encrypted buffer
    if (send(sockfd, encryptedBuffer, encryptedLength, 0) == -1) {
      perror("Error sending encrypted file");
      fclose(file);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
  }
  if (ferror(file)) {
    perror("Error reading file");
    fclose(file);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("File sent successfully\n");

  fclose(file);
  close(sockfd);
}