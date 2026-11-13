//DEPRECATED: SIMPLE EXAMPLE OF POLL (LIBEVENT WAS CHOSEN INSTEAD)

#ifndef POLL_H
#define POLL_H

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

const char *inet_ntop2(void *addr, char *buf, size_t size);

int get_listener_socket(void);

void add_to_pfds(struct pollfd **pfds, int newfd, int *fd_count, int *fd_size);

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count);

void handle_new_connection(int listener, int *fd_count,
                        int *fd_size, struct pollfd **pfds);

void handle_client_data(int listener, int *fd_count,
                        struct pollfd *pfds, int *pfd_i);

void process_connections(int listener, int *fd_count, 
                        int *fd_size, struct pollfd **pfds);

#endif
#endif
