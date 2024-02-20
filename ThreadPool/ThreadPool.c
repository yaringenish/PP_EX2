//
// Created by yarin on 20/02/2024.
//

#include "ThreadPool.h"
#include "TaskQueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void runThreadPool(ThreadPool* tp , int current_level,int numVertices) {
    /* As long as there are tasks in the queue or running threads... */
    while (!isEmpty(tp[current_level].q) || *(tp[current_level].runningThreads) > 0) {
        if(tp[current_level].my_current_threads == 0 && isEmpty(tp[current_level].q))
                break;
        /* If we are not past max number of threads */
        if (*(tp[current_level].runningThreads) < tp[current_level].maxThreads && !isEmpty(tp[current_level].q)) {
            /* Run task */
            TaskData data = pop(tp[current_level].q);

            pthread_t thread;

            if (pthread_create(&thread, NULL, data.function, data.args)) {
                perror("pthread_create failed.");
                exit(1);
            } else {
                tp[current_level].my_current_threads++;
                ++*(tp[current_level].runningThreads);
            }
        }
    }
    if(current_level < numVertices - 1)
        runThreadPool(tp,current_level+1,numVertices);

}