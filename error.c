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


int errorHandle(const char * errmsg, int * sock, struct addrinfo ** bind_addr) {
  if (bind_addr && *bind_addr) {
    freeaddrinfo(*bind_addr);
    *bind_addr = NULL;
  } 
  if (sock && *sock >= 0) {
    close(*sock);
    *sock = -1;
  }
  fprintf(stderr, "ERROR: %s \n", errmsg);
  return -1;
}

