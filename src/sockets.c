#include "sockets.h"

#include "network_init.h"

#ifdef _WIN32
#define SOCKET -1

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

socket_t new_socket(socket_type type)
{
  socket_t new_socket = (socket_t)socket(AF_INET, type, 0);

  return new_socket;
}

