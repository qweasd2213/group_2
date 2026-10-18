#include "threadpool.hpp"
#include <iostream>
#include <pthread.h>
#include <stdexcept>

struct WorkerArgs {
    Threads::ThreadPool *pool;
    unsigned id;
};

namespace Threads {
ThreadPool::ThreadPool(unsigned thread_count) {
    if (thread_count < 1) {
        throw std::invalid_argument("Thread count must be greater than zero");
    }
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
    pthread_cond_init(&completed, nullptr);

    for (unsigned i{0}; i < thread_count; i++) {
        pthread_t thread;
        auto *args = new WorkerArgs{this, i};
        if (pthread_create(&thread, nullptr, worker, args) != 0) {
            delete args;
            throw std::runtime_error("Failed to create the thread");
        }
        workers.push_back(thread);
    }
}

ThreadPool::~ThreadPool() {
    pthread_mutex_lock(&mutex);
    stopping = true;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&mutex);

    for (auto workerThread : workers) {
        pthread_join(workerThread, nullptr);
    }
    pthread_cond_destroy(&completed);
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mutex);
}

void *ThreadPool::worker(void *arg) {
    auto *args = static_cast<WorkerArgs *>(arg);
    auto *pool = args->pool;
    const unsigned id = args->id;
    delete args;

    while (true) {
        Task task;

        pthread_mutex_lock(&pool->mutex);

        while (pool->tasks.empty() && !pool->stopping) {
            pthread_cond_wait(&pool->condition, &pool->mutex);
        }

        if (pool->stopping && pool->tasks.empty()) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }
        task = pool->tasks.front();
        pool->tasks.pop();
        ++pool->active;
        pthread_mutex_unlock(&pool->mutex);
        task.execute();
        pthread_mutex_lock(&pool->mutex);
        --pool->active;
        if (pool->tasks.empty() && pool->active == 0) {
            pthread_cond_broadcast(&pool->completed);
        }

        pthread_mutex_unlock(&pool->mutex);
    }
    return nullptr;
}
void ThreadPool::submit(void *(*func)(void *), void *args) {
    pthread_mutex_lock(&mutex);

    tasks.push(Task{func, args});
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
}

void ThreadPool::wait() {
    pthread_mutex_lock(&mutex);

    while (!tasks.empty() || active > 0) {
        pthread_cond_wait(&completed, &mutex);
    }

    pthread_mutex_unlock(&mutex);
}
} // namespace Threads
