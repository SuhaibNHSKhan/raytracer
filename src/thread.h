#ifndef THREAD_H
#define THREAD_H

#include "win32_ray.c"
#include "util.h"

typedef struct {
    b32 end;
    void* data;
    thread_func func;
} workitem_t;

typedef struct {
    u32 max_size;
    semaphore_t sem_consume;
    semaphore_t sem_produce;
    mutex_t mut;
    u32 st, edp1;
    workitem_t* workitems;
} workerqueue_t;


typedef struct {
    u32 tcount;
    thread_t* threads;
    workerqueue_t* queue;
} threadpool_t;


#endif //THREAD_H
