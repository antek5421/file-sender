#ifndef SERVER_H
#define SERVER_H

#include "hash.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "hash.h"

#define PORT 8080
#define BUFFER_SIZE 1024 * 1024
#define SA struct sockaddr

void server(const char *filename);

#endif