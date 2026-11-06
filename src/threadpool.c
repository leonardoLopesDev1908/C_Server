#include "threadpool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void *thread_work_loop(void *tp){
  t_pool *pool = (t_pool*)tp;
  while(1){
    pthread_mutex_lock(pool->lock);
    while(pool->work_count == 0 && !pool->stop)
      pthread_cond_wait(pool->work_cond, pool->lock);
    if(pool->stop) {
      pool->thread_count--;
      pthread_cond_signal(pool->finished_cond);
      pthread_mutex_unlock(pool->lock);
      break;
    }

    t_work_t *work = t_pool_pop_work(pool);

    pthread_mutex_unlock(pool->lock);
    //work->func(work->arg);
    free(work);
  }

  return NULL;
}

t_pool *t_pool_init(int num)
{
  t_pool *tp = (t_pool*)malloc(sizeof(t_pool));

  if(tp == NULL){
    printf("[ERROR] t_pool alloc\n");
    return NULL;
  }
  
  memset(tp, 0, sizeof(t_pool));
  
  pthread_mutex_init(tp->lock, NULL);
  pthread_cond_init(tp->work_cond, NULL);
  pthread_cond_init(tp->finished_cond, NULL);

  pthread_t thread;

  for(int i = 0; i < num; i++){
    pthread_create(&thread, NULL, thread_work_loop, NULL);
    pthread_detach(thread);
  }
  
  return tp;
}

void t_pool_add_work(t_pool *tp, t_work_t *w)
{
  pthread_mutex_lock(tp->lock);
  t_work_t *last = tp->last_work;
  last->next = w;
  tp->last_work = last->next;

  tp->work_count++;
  
  pthread_mutex_unlock(tp->lock);
}

t_work_t *t_pool_pop_work(t_pool *tp)
{
  pthread_mutex_lock(tp->lock);
  if(tp->work_count == 0) {
    printf("There is no work on the pool\n");
    return NULL;
  }
  t_work_t *work = (t_work_t*)tp->first_work;

  tp->first_work = tp->first_work->next;
  work->next = NULL;
  tp->work_count--;

  if(tp->first_work == NULL)
    tp->last_work = NULL;
  
  pthread_mutex_lock(tp->lock);
  return work;
}

t_work_t *t_pool_wait_and_pop(t_pool *tp)
{
  pthread_mutex_lock(tp->lock);
  while(tp->work_count == 0 && !tp->stop)
    pthread_cond_wait(tp->work_cond, tp->lock);

  t_work_t *work = tp->first_work;
  tp->first_work = tp->first_work->next;
  work->next = NULL;

  tp->work_count--;
  
  pthread_mutex_unlock(tp->lock);
  return work;
}
                             
void t_pool_wait(t_pool *tp)
{
  pthread_cond_wait(tp->work_cond, tp->lock);
}

void t_pool_destroy(t_pool *tp)
{
  pthread_mutex_lock(tp->lock);
  tp->stop = 1;
  
  pthread_mutex_destroy(tp->lock);
  pthread_cond_destroy(tp->work_cond);
  pthread_cond_destroy(tp->finished_cond);
}

void t_proccess_count()
{
  printf("[WARN] Method not implemented yet\n");
}
