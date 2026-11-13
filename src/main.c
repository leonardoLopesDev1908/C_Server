#include <stdio.h>

#include "server.h"

int main(){
    
    printf("Hello, world!\n");
    server_t *server = server_init(1, "127.0.0.1", "6767");
    
    return 0;
}
