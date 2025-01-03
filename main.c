#include "include/client.h"
#include "include/server.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s -s <filename> | -c <IP_address> <filename>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "-s") == 0) {
    if (argc != 3) {
      printf("Error: Server mode requires a filename parameter.\n");
      return 1;
    }
    server(argv[2]);
  } else if (strcmp(argv[1], "-c") == 0) {
    if (argc != 4) {
      printf("Error: Client mode requires both an IP address and a filename "
             "parameter.\n");
      return 1;
    }
    client(argv[2], argv[3]);
  } else {
    printf("Invalid option: %s. Use -s for server or -c for client.\n",
           argv[1]);
    return 1;
  }

  return 0;
}
