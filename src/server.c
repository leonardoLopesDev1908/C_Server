#include <stdio.h>
#include <stdlib.h>

#include "server.h"

#define BACKLOG 10

static server_loop(server_t *server)
{
  while(listen(server->socket, BACKLOG) == 0)
  {
    pthread_mutex_lock(server->lock);
    if(server->stop){
      pthread_mutex_unlock(server->lock);
      return;
    }
    pthread_mutex_unlock(server->lock);
    
    struct sockaddr addr;
    int client_socket;

    if((client_socket = accept(server->socket, addr.ai_addr, addr.ai_addrlen)) != 0);
    {
      printf("[ERROR] accept connection\n");
      return 1;
    }

    t_work_t *new_session = (t_work_t*)malloc(sizeof(t_work_t));

    t_work_t->arg = &client_socket;
    t_work_t->func(session_stream);
    t_work_t->next = NULL;

    t_pool_add_work(server->tp, new_session);    
  }
}

//https://beej.us/guide/bgnet/
server_t *server_init(socket_type type, const char *address, const char *port)
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
//server->parser = parser_init();i

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = type;
  hints.ai_flags = AI_PASSIVE;
  
  if(getaddrinfo(address, port, &hints, &res) != 0)
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
     
  pthread_create(&server->thread, NULL, server_loop, server);
  pthread_detach(server->thread);
  
  return server;
}

void server_stop(server_t *server)
{
  close_socket(server->socket);
  t_pool_destroy(server->tp);
  platform_cleanup();

    //free(server->parser);
  free(tp);

  pthread_mutex_lock(server->lock);
  server->stop = 1;
  pthread_mutex_unlock(server->lock);
    
  printf("Server down\n");
}
