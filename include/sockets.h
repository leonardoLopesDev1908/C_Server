#ifndef SOCKETS_H
#define SOCKETS_H

#include "network_init.h"

#ifdef _WIN32

typedef SOCKET socket_t;
const socket_t INVALID_S = INVALID_SOCKET;

#elif __linux__

#include <sys/types.h>
#include <sys/socket.h>

typedef int socket_t;
const socket_t INVALID_S = -1;

#endif

enum socket_type {
  TCP = 1,
  UDP = 2,
};
socket_t new_socket(socket_type type)
{
  socket_t new_socket = (socket_t)socket(AF_INET, type, 0);

  if(new_socket == INVALID_S){
    printf("[ERROR] invalid socket\n");
    return NULL;
  }

  return new_socket;
}

#ifdef _WIN32

void close_socket(socket_t s) { closesocket(s); }

int is_invalid(socket_t s) { return s == INVALID_SOCKET; }

int is_socket_error(socket_t s) { return s == SOCKET_ERROR; }

#elif __linux__

#include <sys/socket.h>
#include <unistd.h>

void close_socket(socket_t s) { close(s); }

int is_invalid(socket_t s) { return s == -1; }

int is_socket_error(socket_t s) { return s == -1; }

#endif

#endif
