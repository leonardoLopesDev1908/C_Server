//DEPRECATED: SIMPLE EXAMPLE OF POLL (LIBEVENT WAS CHOSEN INSTEAD)


//The following code was developed by Beejs and is available at:
//https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf


#ifdef __linux__
/*
** pollserver.c-- a cheezy multiperson chat server
*/

#include "linux_poll.h"

#define PORT "9034" // Port we're listening on telnet , the server should detect the discon
/*
* Convert socket to IP address string.
* addr: struct sockaddr_in or struct sockaddr_in6
*/
const char *inet_ntop2(void *addr, char *buf, size_t size)
{
    struct sockaddr_storage *sas = addr;
    struct sockaddr_in *sa4;
    struct sockaddr_in6 *sa6;
    void *src;
    switch (sas->ss_family) {
        case AF_INET:
            sa4 = addr;
            src = &(sa4->sin_addr);
            break;
        case AF_INET6:
            sa6 = addr;
            src = &(sa6->sin6_addr);
            break;
        default:
            return NULL;
    }
    return inet_ntop(sas->ss_family, src, buf, size);
}

int get_listener_socket(void)
{
    int listener; // Listening socketdescriptor
    int yes=1; //Forsetsockopt() SO_REUSEADDR,below
    int rv;

    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
       fprintf(stderr, "pollserver: %s\n", gai_strerror(rv));
       exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }
    
    // If wegothere,it means wedidn't get bound
    if (p == NULL) {
        return-1;
    }

    freeaddrinfo(ai); // All donewith this

    // Listen
    if (listen(listener, 10) ==-1) {
        return-1;
    }

    return listener;
}

void add_to_pfds(struct pollfd **pfds, int newfd, int *fd_count, int *fd_size)
{
    if(*fd_count == *fd_size) {
        *fd_size *= 2;
        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; //Checkready-to-read
    (*pfds)[*fd_count].revents = 0;

    (*fd_count)++;
}

void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}

void handle_new_connection(int listener, int *fd_count,
                        int *fd_size, struct pollfd **pfds)
{
    struct sockaddr_storage remoteaddr; //Client address
    socklen_t addrlen;
    int newfd; // Newly accept()edsocket descriptor
    char remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

    if (newfd ==-1) {
        perror("accept");
    } else {
        add_to_pfds(pfds, newfd, fd_count, fd_size);
        printf("pollserver: newconnection from %s onsocket %d\n", inet_ntop2(
                                                                    &remoteaddr, 
                                                                    remoteIP, 
                                                                    sizeof(remoteIP)),
                                                                    newfd
        );
    }
}

void handle_client_data(int listener, int *fd_count,
                        struct pollfd *pfds, int *pfd_i)
{
    char buf[256]; // Bufferforclient data

    int nbytes = recv(pfds[*pfd_i].fd, buf, sizeof buf, 0);

    int sender_fd = pfds[*pfd_i].fd;

    if (nbytes <= 0) { //Goterroror connectionclosedbyclient
        if (nbytes == 0) {
        //Connection closed
        printf("pollserver: socket %d hung up\n", sender_fd);
        } else {
            perror("recv");
        }

        close(pfds[*pfd_i].fd); //Bye!

        del_from_pfds(pfds, *pfd_i, fd_count);

        (*pfd_i)--;

    } else { //Wegotsome gooddata fromaclient
        printf("pollserver:recv fromfd %d: %.*s", sender_fd, nbytes, buf);
        
        for(int j = 0; j < *fd_count; j++) {
            int dest_fd = pfds[j].fd;
            if (dest_fd != listener && dest_fd != sender_fd) {
                if (send(dest_fd, buf, nbytes, 0) ==-1) {
                    perror("send");
                }
            }
        }
    }
}

void process_connections(int listener, int *fd_count, int *fd_size,
                            struct pollfd **pfds)
{
    for(int i = 0; i < *fd_count; i++) {
        // Check ifsomeone'sreadyto read
        if ((*pfds)[i].revents & (POLLIN | POLLHUP)) {
        //We got one!!
            if ((*pfds)[i].fd == listener) {
            //If we're the listener, it'sanewconnection
            handle_new_connection(listener, fd_count, fd_size, pfds);
            } else {
            //Otherwisewe'rejust a regular client
            handle_client_data(listener, fd_count, *pfds, &i);
            }
        }
    }
}

#endif
