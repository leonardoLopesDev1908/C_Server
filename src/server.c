#include <stdio.h>
#include <stdlib.h>

#include "server.h"

static server_loop(server_t *server)
{
  while(!server->stop && listen(server->socket,) == 0)
  {
    //accept

    //create session

    //detach session

    //keeps listening
  }
}

//https://beej.us/guide/bgnet/
server_t *server_init(socket_type type, const char* address, int port)
{
  if(platfform_init() != 0){
    printf("[ERROR] platform_init\n");
    return NULL;
  }
  
  server_t *server = (server_t*)malloc(sizeof(server_t))
  memset(server, 0, sizeof(server_t));
  
  server->socket = new_socket(type);
  
  long max_threads = get_max_threads();
  server->tp = t_pool_init(max_threads);
//server->parser = parser_init();

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = type;
  hints.ai_flags = AI_PASSIVE;

  //WHERE TO PUT PORT AND ADDRESS
  
  if(getaddrinfo(NULL, port, &hints, &res) != 0)
  {
    printf("[ERROR]: getaddrinfo\n");
    freeaddrinfo(res);
    return 1;
  }

  if(bind(server->socket, res->ai_addr, res->ai_addrlen) != 0)
  {
    printf("[ERROR]: bind\n");
    freeaddrinfo(res);
    return 1;
  }

  freeaddrinfo(res);
     
  pthread_create(&server->thread, NULL, server_loop(server), NULL);
  pthread_detach(server->thread);
  
  return server;
}

void server_stop(server_t *server)
{
  close_socket(server->socket);
  t_pool_destroy(server->tp);
  platform_cleanup();

  free(server->socket);
  //free(server->parser);
  free(tp);

  printf("Server down\n");
}
