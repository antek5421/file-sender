#include "client.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h> // Dla atoi()

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <mode> <IP_address> <filename>\n", argv[0]);
    printf("  <mode>: 0 for server, 1 for client\n");
    printf("  <IP_address>: Specify the server IP address (required for client "
           "mode, ignored in server mode)\n");
    printf(
        "  <filename>: Name of the file to send (client) or save (server)\n");
    return 1;
  }

  int mode = atoi(argv[1]);
  if (mode == 0) {
    server(argv[3]);
  } else if (mode == 1) {
    client(argv[2], argv[3]);
  } else {
    printf("Invalid mode: %s. Use 0 for server or 1 for client.\n", argv[1]);
    return 1;
  }

  return 0;
}
