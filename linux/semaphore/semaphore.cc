#include <cstdio>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define QUEUE_MAX 5
class RingQueue
{
 public:
   RingQueue(int maxq = QUEUE_MAX)
     :_queue(maxq)
      , _capacity(maxq)
      , _step_read(0)
      , _step_write(0)
  {
    sem_init(&_lock, 0, 1);  //实现互斥锁
    sem_init(&_sem_data, 0, 0);  //数据空间初始化为0
    sem_init(&_sem_idle , 0, 1);  //空闲空间初始化为maxq
  }
   ~RingQueue()
   {
     sem_destroy(&_lock);
     sem_destroy(&_sem_data);
     sem_destroy(&_sem_idle);
   }
  bool push(const int& data)
  {
    //先判断能否访问在加锁这个顺序，如果相反了，枷锁成功后发现没有空闲结点
    //1.判断是否能访问，不能就阻塞--空闲空闲计数的判断，能访问空闲空间计数-1
    
    sem_wait(&_sem_idle);
    //2.能访问就加锁，保护访问过程
    sem_wait(&_lock);
    //3 资源的访问
    _queue[_step_write] = data;
    _step_write = (_step_write + 1)% _capacity;
    //4.解锁
    sem_post(&_lock); //lock计数+1 唤醒因加锁而阻塞的线程
    //5. 入队数据之后，数据空间计数+1 唤醒消费者
    sem_post(&_sem_data);
    return true;
  }
  bool pop(int* data)
  {
    sem_wait(&_sem_data); //判断数据空间是否能访问
    sem_wait(&_lock);     //有数据则枷锁保护访问数据的过程
    *data = _queue[_step_read];
    _step_read = (_step_read + 1)% _capacity;
    sem_post(&_lock); //解锁
    sem_post(&_sem_idle); //唤醒生产者
    return true;
  }

 private:
  vector<int> _queue;
  int _capacity;
  int _step_read;
  int _step_write;

  sem_t _lock;
  sem_t _sem_data;
  sem_t _sem_idle;
};
void* thr_productor(void* arg)
{
  RingQueue* queue = (RingQueue*)arg;
  int i = 0;
  while(1)
  {
    queue->push(i);
    printf("productor push data:%d\n", i++);
  }
  return NULL;
}
void* thr_customer(void* arg)
{
  RingQueue* queue = (RingQueue*)arg;
  while(1)
  {
    int data;
    queue->pop(&data);
    printf("customer pop data:%d\n",data);
  }
  return NULL;
}
int main()
{
  int ret, i ;
  pthread_t ptid[4], ctid[4];
  RingQueue queue;

  for(i = 0; i < 4; ++i)
  {
    ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&queue);
    if(ret != 0)
    {
      printf("create productor thread error\n");
      return -1;
    }

    ret = pthread_create(&ctid[i],NULL, thr_customer, (void*)&queue);
    if(ret != 0)
    {
      printf("create productor thread error\n");
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
