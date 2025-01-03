#include "../include/server.h"

void server(const char *filename) {
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;
  char buffer[BUFFER_SIZE];
  ssize_t bytesRead;
  unsigned char receivedHash[SHA256_DIGEST_LENGTH];
  unsigned char calculatedHash[SHA256_DIGEST_LENGTH];
  EVP_MD_CTX *mdctx = NULL;
  unsigned char key[AES_KEY_SIZE]; // Klucz AES
  unsigned char iv[AES_BLOCK_SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    perror("Socket bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, 5) != 0) {
    perror("Listen failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  socklen_t len = sizeof(cli);
  connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
  if (connfd < 0) {
    perror("Server accept failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &cli.sin_addr, client_ip, INET_ADDRSTRLEN);
  printf("Connection established with client: %s:%d\n", client_ip,
         ntohs(cli.sin_port));

  char input[3];
  printf("Do you want to accept the file from this IP? [y/n]: ");
  fgets(input, sizeof(input), stdin);
  if (input[0] != 'y' && input[0] != 'Y') {
    printf("You rejected the file.\n");
    close(connfd);
    close(sockfd);
    return;
  }

  printf("Receiving file hash...\n");
  bytesRead = recv(connfd, receivedHash, SHA256_DIGEST_LENGTH, 0);
  if (bytesRead != SHA256_DIGEST_LENGTH) {
    perror("Error receiving hash");
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }
  printf("Hash received from client.\n");

  // Odbieranie klucza AES
  bytesRead = recv(connfd, key, AES_KEY_SIZE, 0);
  if (bytesRead != AES_KEY_SIZE) {
    perror("Error receiving AES key");
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Odbieranie wektora IV
  bytesRead = recv(connfd, iv, AES_BLOCK_SIZE, 0);
  if (bytesRead != AES_BLOCK_SIZE) {
    perror("Error receiving IV");
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Error opening file");
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    perror("Error creating digest context");
    fclose(file);
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
    perror("Error initializing digest");
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Receiving file...\n");
  while ((bytesRead = recv(connfd, buffer, sizeof(buffer), 0)) > 0) {
    // Odszyfrowanie danych przed zapisaniem do pliku
    unsigned char decryptedBuffer[BUFFER_SIZE];
    int decryptedLength = aes_decrypt_buffer((unsigned char *)buffer, bytesRead,
                                             key, iv, decryptedBuffer);

    if (decryptedLength < 0) {
      perror("Error decrypting file data");
      EVP_MD_CTX_free(mdctx);
      fclose(file);
      close(connfd);
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    fwrite(decryptedBuffer, sizeof(char), decryptedLength, file);

    if (EVP_DigestUpdate(mdctx, decryptedBuffer, decryptedLength) != 1) {
      perror("Error updating digest");
      EVP_MD_CTX_free(mdctx);
      fclose(file);
      close(connfd);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
  }

  if (bytesRead < 0) {
    perror("Error receiving file");
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("File received successfully\n");
  fclose(file);

  unsigned int hashLength;
  if (EVP_DigestFinal_ex(mdctx, calculatedHash, &hashLength) != 1) {
    perror("Error finalizing digest");
    EVP_MD_CTX_free(mdctx);
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  EVP_MD_CTX_free(mdctx);

  if (compare_hashes(receivedHash, calculatedHash, SHA256_DIGEST_LENGTH)) {
    printf("Hash matches. File integrity verified.\n");
  } else {
    printf("Hash mismatch! File may be corrupted.\n");
  }

  close(connfd);
  close(sockfd);
}
