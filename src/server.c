#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <netdb.h>

#include "server.h"
#include "system_info.h"

#define BACKLOG 10

typedef struct {
  server_t *server;
  struct addrinfo *addr;
} session_info_t;

static void *server_loop(void *args)
{
  session_info_t *session = (session_info_t*)args;
  while(listen(session->server->socket, BACKLOG) == 0)
  {
    pthread_mutex_lock(session->server->lock);
    if(session->server->stop){
      pthread_mutex_unlock(session->server->lock);
      return NULL;
    }
    pthread_mutex_unlock(session->server->lock);
    
    int client_socket;

    if((client_socket = accept(
                               session->server->socket,
                               session->addr->ai_addr,
                               NULL)) != 0);
    {
      printf("[ERROR] accept connection\n");
      return NULL;
    }

    t_work_t *new_session = (t_work_t*)malloc(sizeof(t_work_t));

    new_session->arg = &client_socket;
    //new_session->func(session_stream);
    new_session->next = NULL;

    t_pool_add_work(session->server->tp, new_session);    
  }

  return NULL;
}

//https://beej.us/guide/bgnet/
server_t *server_init(socket_type type, const char *address, const char *port)
{
  if(platform_init() != 0){
    printf("[ERROR] platform_init\n");
    return NULL;
  }
  
  server_t *server = (server_t*)malloc(sizeof(server_t));
  memset(server, 0, sizeof(server_t));
  
  server->socket = new_socket(type);

  if(server->socket == -1){
    printf("[ERROR] invalid new socket\n");
    return NULL;
  }
  
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
    return NULL;
  }

  if(bind(server->socket, res->ai_addr, res->ai_addrlen) != 0)
  {
    printf("[ERROR]: bind\n");
    freeaddrinfo(res);
    return NULL;
  };

  session_info_t *session_info = (session_info_t*)malloc(sizeof(session_info_t));
  
  session_info->server = server;
  session_info->addr = res;
  
  pthread_create(&server->thread, NULL, server_loop, (void*)session_info);
  pthread_detach(server->thread);
  
  return server;
}

void server_stop(server_t *server)
{
  close_socket(server->socket);
  t_pool_destroy(server->tp);
  platform_cleanup();

    //free(server->parser);
  free(server->tp);

  pthread_mutex_lock(server->lock);
  server->stop = 1;
  pthread_mutex_unlock(server->lock);
    
  printf("Server down\n");
}
