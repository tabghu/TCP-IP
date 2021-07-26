//main.cc
#include <unistd.h>
#include "TheadPool.hpp"



//处理方法1 
void test_func(int data)
{
  int sec = (data % 3) + 1;
  printf("tid :%p -- get data: %d, sleep: %d\n", pthread_self(), data, sec);
  sleep(sec);
}

void tmp_func(int data)
{
  printf("tid: %p -- tmp_func\n", pthread_self());
  sleep(1);
}

int main()
{
  ThreadPool pool;
  for(int i = 0; i < 10; ++i)
  {
    ThreadTask task;
    if(i % 2 == 0)
    {
      task.setTask(i, test_func);
    }
    else
    {
      task.setTask(i, tmp_func);
    }
    pool.takePush(task);
  }
  sleep(1000);
  return 0;
}
