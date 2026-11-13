#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

list_t *list_t_create()
{
    list_t *list = (list_t*)malloc(sizeof(list_t) * LIST_MAX_SIZE);
    list->count = 0;

    memset(list, 0, sizeof(list_t));
    pthread_mutex_init(&list->lock, NULL);

    return list;
}

list_t *list_t_load_file(const char* filename)
{
    list_t *list = (list_t*)malloc(sizeof(list_t) * LIST_MAX_SIZE); 
    
    FILE *file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("[ERROR] fopen\n");
        return NULL;
    }

    char line[256];
    
    while(fgets(line, sizeof(line), file) != NULL)
    {
        if(strcmp(line, "\n") == 0)
            continue;
        
        int division = strcspn(line, ":");
        int end = strcspn(line, "\n");

        node_t *new_node = (node_t*)malloc(sizeof(node_t));

        strncpy(new_node->key, line, division - 1);
        strncpy(new_node->value, line + division + 1, end-1);
        
        list_t_add_node(list, new_node);
    }

    fclose(file);

    return list;
}

void list_t_destroy(list_t *table)
{
    pthread_mutex_destroy(&table->lock);
    free(table);
}

int list_t_add(list_t *table, const char *key, void *value)
{
    return 0;
}

int list_t_add_node(list_t *table, node_t *node)
{
    if(table->count == LIST_MAX_SIZE)
    {
        printf("[ERROR] List %c is full\n", node->key[0]);
        return -1;
    }

    if(table->first_node == NULL)
    {
        table->first_node = node;
    } else {
        node_t *last = table->last_node;
        last->next = node;
        table->last_node = node;
    }
    
    table->count++;
    return 0;
}
