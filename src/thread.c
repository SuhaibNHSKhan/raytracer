// thread

#include "thread.h"
/*
workerqueue_t workerqueue_new(u32 max_items) {
    workerqueue_t queue;
    queue.semaphore = sem_create(0, max_items);
    // TODO(suhaibnk): This could be very error prone check if ok???
    // Porbably fine since we are limiting overrun using the semaphore not the indices
    queue.st = 0;
    queue.edp1 = 0;
    queue.workitems = malloc(sizeof(workerqueue_t) * max_items);
    
    return queue;
}
*/

dword thread_worker(void* data);

void workerqueue_init(workerqueue_t* queue, u32 max_items) {
    queue->max_size = max_items;
    queue->sem_consume = sem_create(0, max_items);
    queue->sem_produce = sem_create(max_items, max_items);
    queue->mut = mut_create();
    queue->st = 0;
    queue->edp1 = 0;
    queue->workitems = (workitem_t*) malloc(sizeof(workerqueue_t) * max_items);
}

workitem_t workerqueue_get(workerqueue_t* queue) {
    
    sem_wait(&queue->sem_consume);
    
    mut_lock(&queue->mut);
    
    workitem_t workitem = queue->workitems[queue->st];
    
    queue->st = (queue->st + 1) % queue->max_size;
    
    mut_unlock(&queue->mut);
    
    sem_release(&queue->sem_produce);
    
    return workitem;
}


void workerqueue_push(workerqueue_t* queue, workitem_t item) {
    
    sem_wait(&queue->sem_produce);
    
    mut_lock(&queue->mut);
    
    queue->workitems[queue->edp1] = item;
    
    queue->edp1 = (queue->edp1 + 1) % queue->max_size;
    
    mut_unlock(&queue->mut);
    
    sem_release(&queue->sem_consume);
}

threadpool_t threadpool_new(u32 max_threads) {
    threadpool_t threadpool;
    threadpool.tcount = max_threads;
    threadpool.threads = malloc(sizeof(threadpool_t) * max_threads);
    threadpool.queue = malloc(sizeof(workerqueue_t));
    
    workerqueue_init(threadpool.queue, max_threads * 2);
    
    for (u32 i = 0; i < max_threads; ++i) {
        threadpool.threads[i] = thread_run(thread_worker, threadpool.queue);
    }
    
    return threadpool;
}

void threadpool_submit(threadpool_t* threadpool, thread_func func, void* data) {
    workitem_t workitem;
    
    workitem.end = 0;
    workitem.data = data;
    workitem.func = func;
    
    workerqueue_push(threadpool->queue, workitem);
}

void threadpool_end(threadpool_t* threadpool) {
    workitem_t end_item = {
        .end = 1
    };
    
    for (u32 i = 0; i < threadpool->tcount; ++i) {
        workerqueue_push(threadpool->queue, end_item);
    }
    
    thread_wait_for_all(threadpool->tcount, threadpool->threads, 1, INFINITE);
}

dword threadpool_complete(threadpool_t* threadpool, dword ms) {
    return thread_wait_for_all(threadpool->tcount, threadpool->threads, 1, ms);
}

dword thread_worker(void* data) {
    workerqueue_t* queue = (workerqueue_t*) data;
    
    while (1) {
        workitem_t workitem = workerqueue_get(queue);
        
        if (workitem.end) {
            break;
        }
        
        workitem.func(workitem.data);
    }
    
    return 0;
}