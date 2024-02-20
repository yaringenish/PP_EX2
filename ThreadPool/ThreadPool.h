//
// Created by yarin on 20/02/2024.
//

#ifndef UNTITLED5_THREADPOOL_H
#define UNTITLED5_THREADPOOL_H
#include "TaskQueue.h"
#include <stdatomic.h>
#include <pthread.h>

struct ThreadPool {
    unsigned int maxThreads;
    int my_current_threads;
    atomic_int* runningThreads;
    TaskQueue *q;
};

typedef struct ThreadPool ThreadPool;

void runThreadPool(ThreadPool *tp,int current_level,int numVertices);
#endif //UNTITLED5_THREADPOOL_H
