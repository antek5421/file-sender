#include "client.h"
#include <openssl/sha.h>

void client(const char *IP, const char *filePath) {
  int sockfd;
  struct sockaddr_in servaddr;
  char buffer[BUFFER_SIZE];
  size_t bytesRead;

  unsigned char hash[SHA256_DIGEST_LENGTH];
  calculate_file_hash(filePath, hash);

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

  FILE *file = fopen(filePath, "rb");
  if (file == NULL) {
    perror("Error opening file");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Sending file: %s to server\n", filePath);

  while ((bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
    if (send(sockfd, buffer, bytesRead, 0) == -1) {
      perror("Error sending file");
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