#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "hash.h"

#define PORT 8080
#define BUFFER_SIZE 1024 * 1024
#define SA struct sockaddr

void client(const char *IP, const char *filePath);

#endif