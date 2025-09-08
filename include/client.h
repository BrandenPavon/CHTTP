#ifndef CLIENT_H
#define CLIENT_H

ssize_t send_all(int sock, const char * buff, size_t len, int flags);
void handleClient(int socket_client, int sock_listen, struct sockaddr *client_address, socklen_t * client_len);

#endif
