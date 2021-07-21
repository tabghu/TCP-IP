#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void* thread_main(void* arg);

int main(int argc, char* argv[])
{
  pthread_t t_id;
  int thread_param = 5;
  void* th_ret;
  if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
  {
    puts("pthread_create() error");
    return -1;
  }
  if(pthread_join(t_id, &th_ret) != 0)
  {
    puts("pthread_join error!");
    return -1;
  }
  printf("Thread return message : %s \n", (char*)th_ret);
  free(th_ret);
  return 0;
}

void* thread_main(void* arg)
{
  int i;
  int cnt = *((int*)arg);
  char* msg = (char*)malloc(sizeof(char) * 50);
  strcpy(msg, "Hellom i am thread \n");
  for(i = 0; i < cnt; ++i)
  {
    sleep(1);
    puts("running thread");
  }
  return (void*)msg;
}
