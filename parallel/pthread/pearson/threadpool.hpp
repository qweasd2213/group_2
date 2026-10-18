#if !defined(THREADPOOL_HPP)
#define THREADPOOL_HPP

#include "task.hpp"
#include <pthread.h>
#include <queue>
namespace Threads {
class ThreadPool {
  private:
    std::vector<pthread_t> workers;
    std::queue<Task> tasks;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    pthread_cond_t completed;
    unsigned active = 0;
    bool stopping = false;

    static void *worker(void *arg);

  public:
    explicit ThreadPool(unsigned thread_count);
    ~ThreadPool();

    void submit(void *(*func)(void *), void *args);
    void wait();
};
} // namespace Threads
#endif
