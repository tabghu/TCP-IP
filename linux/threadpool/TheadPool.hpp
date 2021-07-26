#include <iostream>
#include <cstdio>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

typedef void (*handler_t)(int);
#define MAX_THREAD 5
//任务类
class ThreadTask
{
public:
  ThreadTask()
  {}
  //将数据与处理方式打包在一起
  void setTask(int data, handler_t handler)
  {
    _data = data;
    _handler = handler;
  }

  //执行任务函数
  void run()
  {
    return _handler(_data);
  }
private:
  int _data;
  handler_t _handler;
};

class ThreadPool
{
public:
  ThreadPool(int thr_max = MAX_THREAD):_thr_max(thr_max)
  {
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);


    for(int i = 0; i < _thr_max; ++i)
    {
      pthread_t tid;
      int ret = pthread_create(&tid, NULL, thr_start, this);
      if(ret != 0)
      {
        cout << "create thread error" << endl;
        exit(-1);
      }
    }
  }
  ~ThreadPool()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
  }

  bool takePush(ThreadTask &task)
  {
    pthread_mutex_lock(&_mutex);
    _queue.push(task);
    pthread_mutex_unlock(&_mutex);

    pthread_cond_signal(&_cond);
    return true;
  }

  //类的成员函数 有默认的隐藏参数this指针
  //置为static 没有this指针
  static void* thr_start(void* arg)
  {
    ThreadPool *p = (ThreadPool*)arg;
    while(1)
    {
      pthread_mutex_lock(&p->_mutex);
      while(p->_queue.empty())
      {
        pthread_cond_wait(&p->_cond, &p->_mutex);
      }
      ThreadTask task;
      task = p->_queue.front();
      p->_queue.pop();
      pthread_mutex_unlock(&p->_mutex);
      task.run();
    }
    return NULL;
  }
private:
  int _thr_max; //线程池中最大线程数
  queue<ThreadTask> _queue; //任务缓冲队列
  pthread_mutex_t _mutex; //保护队列操作的互斥量
  pthread_cond_t _cond;   //实现从队列中获取节点的同步条件变量
};
