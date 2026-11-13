#include <stdlib.h>

#include "repository.h"

char* repo_get_phone(const char* name) 
{
    char path[100];
    snprintf(path, sizeof(path), "database/%c.txt", name[0]);

    FILE *file = fopen(path, "r");

    if(file == NULL)
    {
        printf("[ERROR] fopen file\n");
        return NULL;
    }
    


}

int repo_save_phone(const char* name, const char* phone)
{

}

