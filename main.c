#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "headers.h"
#include "client.h"
#include "error.h"


int setupSocket() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  struct addrinfo *bind_address;
  int r = getaddrinfo(0, "8089", &hints, &bind_address);
  if (r != 0) {
    return errorHandle("getaddrinfo failed", 0, 0);
  }
  int socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if ((socket_listen < 0)) {
    return errorHandle("socket creation failed", &socket_listen, &bind_address);
  }
  int yes = 1;
  if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
    return errorHandle("socket reuse failed", &socket_listen, &bind_address);
  }
  if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen) < 0) {
    return errorHandle("bind failed", &socket_listen, &bind_address);
  }
  if (listen(socket_listen, 10) < 0) {
    return errorHandle("listen failed", &socket_listen, &bind_address);
  }
  freeaddrinfo(bind_address);
  return socket_listen;
}

int main() {
  int socket_listen = setupSocket();
  if (socket_listen < 0) {
    return -1;
  }
  printf("socket_listen = %d \n", socket_listen);
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  int socket_client;
  
  while(1) {
    client_len = sizeof(client_address);
    socket_client = accept(socket_listen, (struct sockaddr *) &client_address, &client_len);
    if (socket_client < 0) {
      perror("accept failed");
      continue;
    }
    handleClient(socket_client, socket_listen, (struct sockaddr *) &client_address, &client_len);
  }
  
  close(socket_listen);
  return 0; 
}
