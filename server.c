#include "server.h"

void server(const char *filename) {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;
  char buffer[BUFFER_SIZE];
  int bytesRead;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
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

  len = sizeof(cli);
  connfd = accept(sockfd, (SA *)&cli, &len);
  if (connfd < 0) {
    perror("Server accept failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &cli.sin_addr, client_ip, INET_ADDRSTRLEN);
  printf("Connection established with client: %s:%d\n", client_ip,
         ntohs(cli.sin_port));

  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Error opening file");
    close(connfd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  while ((bytesRead = recv(connfd, buffer, sizeof(buffer), 0)) > 0) {
    if (fwrite(buffer, sizeof(char), bytesRead, file) != (size_t)bytesRead) {
      perror("File write error");
      fclose(file);
      close(connfd);
      close(sockfd);
      exit(EXIT_FAILURE);
    }
  }

  if (bytesRead < 0) {
    perror("Error receiving file");
  } else {
    printf("File received successfully\n");
  }

  fclose(file);
  close(connfd);
  close(sockfd);
}