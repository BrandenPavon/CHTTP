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
#include "error.h"

#define REQBUFFER 8192

char * parse_path() {return NULL;}

int recvValid(int sock, char * req, int reqbuff, int flags) {
  int bytes_recv = recv(sock, req, reqbuff-1, 0);
  if (bytes_recv > 0 && bytes_recv < REQBUFFER) {
    req[bytes_recv] = '\0';
  } else {
    return errorHandle("Buffer too large or small", &sock, 0);
  }
  printf("Recived: %d bytes \n", bytes_recv);
  return 0;
}

ssize_t send_all(int sock, const char * buff, size_t len, int flags) {
  const char *p = buff;
  size_t total = 0;
  while (total < len) {
      ssize_t n = send(sock, p + total, len - total, flags);
      if (n < 0) {
          if (errno == EINTR) continue;
          if (errno == EAGAIN || errno == EWOULDBLOCK) {
              continue;
          }
          return -1;
      }
      if (n == 0) {
          return total;
      }
      total += (size_t)n;
  }
  return total;
}

int handleClient(int socket_client, int sock_listen, struct sockaddr * client_address, socklen_t * client_len) {
  char add_buff[128];
  char request[REQBUFFER];
  FILE *fp;
  getnameinfo(client_address, *client_len, add_buff, sizeof(add_buff), 0, 0, NI_NUMERICHOST);
  recvValid(socket_client, request, REQBUFFER-1, 0);

  fp = fopen("index.html", "rb");
  if (!fp) {
    send_all(socket_client, not_found_response, strlen(not_found_response), 0);
    return errorHandle("FILE NOT FOUND", &socket_client, 0);
  }
  //handle send error handling later
  if (send_all(socket_client, okresponse, strlen(okresponse), 0) < 0) {
    return errorHandle("Send client ok response failed", &socket_client, 0);
  }
  char buf[1024];
  size_t n;
  int total = 0;
  int sent;
  while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
    sent = send_all(socket_client, buf, n, 0);
    total += sent;
    if (sent < 0) {
      errorHandle("Send client packets failed", &socket_client, 0);
      break;
    }
  }
  printf("sent %d bytes \n", total);
  fclose(fp);
  close(socket_client);
  return 1;
}

