#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define  NUM_THREAD 100
void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0;
//声明互斥量
pthread_mutex_t mute;
int main(int argc, char* argv[])
{
  pthread_t thread_id[NUM_THREAD];
  int i;

  //初始化互斥量
  pthread_mutex_init(&mute, NULL);
  printf("sizeof long long : %d\n", sizeof(long long));
  for(i = 0; i < NUM_THREAD; ++i)
  {
    if(i % 2)
      pthread_create(&thread_id[i], NULL, thread_inc, NULL);
    else
      pthread_create(&thread_id[i], NULL, thread_des, NULL);

  }
  for(i = 0; i < NUM_THREAD; ++i)
  {
    pthread_join(thread_id[i], NULL);
  }
  printf("num = %lld \n", num);
  //销毁互斥量
  pthread_mutex_destroy(&mute);
  return 0;
}
void* thread_inc(void* arg)
{
  int i;
  pthread_mutex_lock(&mute);
  for(i = 0; i < 50000000; ++i)
  {
    num  += 1;
  }
  pthread_mutex_unlock(&mute);
  return NULL;
}
void* thread_des(void* arg)
{
  int i;
  for(i = 0; i < 50000000; ++i)
  {
    pthread_mutex_lock(&mute);
    num -= 1;
    pthread_mutex_unlock(&mute);
  }
  return NULL;
}
