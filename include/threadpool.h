#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#define MAX_THREAD_NUMBER 8

typedef void (*t_func)(void *);

typedef struct s_work {
  void *arg;
  t_func func;
  struct s_work *next;
} t_work_t;

typedef struct {
  t_work_t *first_work;
  t_work_t *last_work;
  int work_count;
  int thread_count;
  int stop;
  pthread_mutex_t *lock;
  pthread_cond_t *work_cond;
  pthread_cond_t *finished_cond;
} t_pool;


t_pool *t_pool_init(int num);
void t_pool_add_work(t_pool *tp, t_work_t *w);
t_work_t *t_pool_pop_work(t_pool *tp);
t_work_t *t_pool_wait_and_pop(t_pool *tp);
void t_pool_wait(t_pool *tp);
void t_pool_destroy(t_pool *tp);
void t_proccess_count();
  
#endif
