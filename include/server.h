#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "parser.h"
#include "session.h"
#include "sockets.h"

typedef struct{
  socket_t socket;
  t_pool *tp;
  parser_t *parser;
  
  int running;
  int client_count;

  pthread_t thread;
  pthread_mutex_t *lock;
} server_t;

server_t *server_init(socket_type type, const char* address, short port);
void server_stop();


#endif
