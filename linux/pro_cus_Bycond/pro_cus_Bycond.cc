#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <pthread.h>

using namespace std;
#define QUEUE_MAX 5
class BlockQueue
{
public:
  BlockQueue(int maxq = QUEUE_MAX):_capacity(maxq)
  {
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_pro_cond, NULL);
    pthread_cond_init(&_cus_cond, NULL);
  }
  ~BlockQueue()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_pro_cond);
    pthread_cond_destroy(&_cus_cond);
  }
  bool push(const int& data)
  {
    //生产者将数据入队，若数据满了则需要阻塞
    pthread_mutex_lock(&_mutex);
    while(_queue.size() == _capacity)
    {
      pthread_cond_wait(&_pro_cond, &_mutex);
    }
    //将数据入队
    _queue.push(data);
    //解锁
    pthread_mutex_unlock(&_mutex);
    //唤醒消费者线程
    pthread_cond_signal(&_cus_cond);
    return true;
  }

  bool pop(int* data)
  {
    //消费者将数据出队，若没有数据则需要阻塞
    pthread_mutex_lock(&_mutex);
    while(_queue.empty())
    {
      pthread_cond_wait(&_cus_cond, &_mutex);
    }
    //获取队头元素
    *data = _queue.front();
    _queue.pop();
    //解锁
    pthread_mutex_unlock(&_mutex);
    //唤醒生产者
    pthread_cond_signal(&_pro_cond);
    return true;
  }
private:
  queue<int> _queue;  //简单队列
  int _capacity;      //最大节点数量
  pthread_mutex_t _mutex; //互斥变量
  pthread_cond_t _pro_cond; //生产者条件变量
  pthread_cond_t _cus_cond; //消费者条件变量


};

void* thr_productor(void* arg)
{
  BlockQueue* queue = (BlockQueue*)arg;
  int i = 0;
  while(1)
  {
    //生产者生产数据
    queue->push(i);
    printf("productor push data: %d\n", i++);
  }
  return NULL;
}

void* thr_customer(void* arg)
{
  BlockQueue* queue = (BlockQueue*)arg;
  int i = 0;
  while(1)
  {
    //生产者生产数据
    int data;
    queue->pop(&data);
    printf("customer pop data: %d\n", data);
  }
  return NULL;
}
int main()
{
  int ret, i;
  pthread_t ptid[4], ctid[4];
  BlockQueue queue;
  for(i = 0; i < 4; ++i)
  {
    ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&queue);
    if(ret != 0)
    {
      printf("create productor thread error \n");
      return -1;
    }
    
    ret = pthread_create(&ctid[i], NULL, thr_customer, (void*)&queue);
    if(ret != 0)
    {
      printf("create customer thread error\n");
      return -1;
    }
  }
  for(i = 0; i < 4; ++i)
  {
    pthread_join(ptid[i], NULL);
    pthread_join(ctid[i], NULL);
  }
  return 0;
}
