#include "threadpool.>

int main(){

  tpool *pool = t_pool_init(10);

  if(pool == NULL){
    printf("[ERROR] pool = NULL\n");
  }

  printf("pool is correct\n");

  return 0;
}
