#ifndef SESSION_H
#define SESSION_H

#include "sockets.h"

#define MAX_BUFFER_SIZE 1024

typedef struct {
    int id;
    socket_t socket;
    char *buffer;

} session_t;
    
session_t* session_init(socket_t socket, int id);
//stop development to study poll and libevent
#endif
