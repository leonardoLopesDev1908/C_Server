#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define LIST_MAX_SIZE 16

typedef struct node_s {
    char *key;
    char *value;
    struct node_s *next; 
} node_t;

typedef struct {
    node_t *first_node;
    node_t *last_node;
   
    //time_t last_access maybe future cache implementation;
    pthread_mutex_t lock;
    int count;
} list_t;

list_t *list_t_create(void);

list_t *list_t_load_file(const char *filename);

void list_t_destroy(list_t *table);

void *list_t_get(list_t *table, const char *key);

int list_t_add(list_t *table, const char *key, void *value);

int list_t_add_node(list_t *table, node_t *node);

size_t list_t_length(list_t *table);

typedef struct {
    const char *key; 
    void *value;     

    list_t *_table;       
    size_t _index;   
} list_ti;

list_ti list_t_iterator(list_t *table);

int list_t_next(list_ti *it);

#endif

