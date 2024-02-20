#include "../SyncGraph/graph.h"
#include "../ThreadPool/ThreadPool.h"
#include "../ThreadPool/TaskQueue.h"

struct bfsArgs {
    vertex root;
    Graph *graph;
    vertex v;
    ThreadPool *q;
    int** m;
    int level;
};
typedef struct bfsArgs bfsArgs;

ThreadPool** pools;

void * parallel_bfs_visit(void *args) {
    /* Extract arguments */
    bfsArgs *data = (bfsArgs *) args;
    Graph *graph = data->graph;
    vertex v = data->v;
//    ThreadPool *q = data->q;
    int level = data->level;
    vertex root = data->root;
    int ** m = data->m;
    free(data);

    /* Log the visit */
    printf("%d\n", v);

    /* Start the visit */
    node *neighborsPtr = graph->adjacencyLists[v];

    while (neighborsPtr != NULL) {
        vertex neighbor = neighborsPtr->v;
        /* Synchronously increment the number of visits for the neighbor */
        pthread_mutex_lock(&graph->num_visits_mutexes[neighbor]);
        int neighborVisits = graph->num_visits[root][neighbor]++;
        pthread_mutex_unlock(&graph->num_visits_mutexes[neighbor]);

        /* If the neighbor has not been visited, add a new task to the queue */
        if (neighborVisits == 0) {
            bfsArgs *params = malloc(sizeof(bfsArgs));
            params->graph = graph;
            params->v = neighbor;
//            params->q = q;
            params->level = level+1;
            params->root = root;
            m[root][neighbor] = level+1;
            TaskData td = {parallel_bfs_visit, params};
            insert(pools[root][level+1].q, td);
        }
        neighborsPtr = neighborsPtr->next;
    }
    /* Decrement the number of running threads */
    --*(pools[root][level].runningThreads);
        pools[root][level].my_current_threads--;
}



void init_pools(int numVertices,atomic_int* runningThreads){
    for(int i = 0 ; i < numVertices ; i ++){
        pools[i] = malloc(numVertices*sizeof(ThreadPool));
        for(int j = 0 ; j < numVertices ; j++) {
            TaskQueue *q = malloc(sizeof(TaskQueue));
            initQueue(q);
            pools[i][j].my_current_threads = 0;
            pools[i][j].maxThreads = 1;
            pools[i][j].runningThreads = runningThreads;
            pools[i][j].q = q;
        }
    }
}


void parallel_bfs(Graph *graph,int **m) {
    int numVertices = graph->numVertices;
    atomic_int* runningThreads = malloc(sizeof(atomic_int));
    *runningThreads = 0;
    pools = malloc(numVertices*sizeof(ThreadPool*));
    init_pools(numVertices,runningThreads);
    for (vertex v = 0; v < numVertices; v++) {
        printf("\nDFS Traversal: ");
        /* Insert the root task to the queue */
        bfsArgs* args = malloc(sizeof(bfsArgs));
        args->graph = graph;
        args->v = v;
        args->level = 0;
        args->root = v;
        args->m = m;
        pthread_mutex_lock(&graph->num_visits_mutexes[v]);
        graph->num_visits[v][v]++;
        pthread_mutex_unlock(&graph->num_visits_mutexes[v]);
        TaskData td = {parallel_bfs_visit, args};
        insert((pools[v][0].q), td);
        runThreadPool(pools[v],0,numVertices);
    }
}

void init_matrix(int numVertices,int** m){
    for(int i = 0; i < numVertices; i++){
        for(int j = 0; j < numVertices ; j++){
            if(i == j)
                m[i][j] = 0;
            else
                m[i][j] = -1;
        }
    }
}

void bfs(Graph *graph, int **m){
    int numVertices = graph->numVertices;
    init_matrix(numVertices,m);
    parallel_bfs(graph,m);
}



//dfsArgs *args;
///* Init thread pool */
//TaskQueue q;
//initQueue(&q);
//
//ThreadPool pool;
//pool.maxThreads = 4;
//pool.runningThreads = 0;
//pool.q = &q;
