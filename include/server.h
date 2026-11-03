#ifndef SERVER_H
#define SERVER_H


#include "parser.h"
#include "session.h"
#include "sockets.h"

typedef struct{
  socket_t *socket;
  t_pool *tp;
  parser_t *parser;
  
  int running;
  int client_count;
} server_t;

static server_loop()
{
  while(running)
  {
      
  } 
}

server_t *server_init(socket_type type, const char* address, short port)
{
  if(platfform_init() != 0){
    printf("[ERROR] platform_init\n");
    return NULL;
  }
  
  server_t *server = (server_t*)malloc(sizeof(server_t));

  server->socket = new_socket(type);

  long max_threads = get_max_threads();
  server->tp = t_pool_init(max_threads);
//server->parser = parser_init();

  if(bind())
  
  return server;
}

void server_stop()
{
  close_socket(*socket);
  t_pool_destroy(*tp);
  platform_cleanup();

  free(socket);
  free(parser);
  free(tp);

  printf("Server down\n");
}

#endif
