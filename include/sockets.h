#ifndef SOCKETS_H
#define SOCKETS_H

#include "network_init.h"

#ifdef _WIN32
typedef SOCKET socket_t;

#elif __linux__

typedef int socket_t;

#endif

void close_socket(socket_t s);

int is_invalid(socket_t s);

int is_socket_error(socket_t s);

typedef enum {
  TCP = 1,
  UDP = 2,
} socket_type;
  
socket_t new_socket(socket_type type);

#endif
