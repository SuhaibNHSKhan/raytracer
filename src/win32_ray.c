#ifndef WIN32_RAY_C
#define WIN32_RAY_C

#include <windows.h>
#include "util.h"

#define WAIT_INFINITE INFINITE

typedef struct {
    HANDLE handle;
} thread_t;

typedef struct {
    HANDLE handle;
} semaphore_t;

semaphore_t sem_create(u32 init_val, u32 max_val) {
    semaphore_t sem;
    
    sem.handle = CreateSemaphore(NULL, init_val, max_val, NULL);
    
    return sem;
}

void sem_wait(semaphore_t* sem) {
    WaitForSingleObject(sem->handle, WAIT_INFINITE);
}

void sem_release(semaphore_t* sem) {
    ReleaseSemaphore(sem->handle, 1, NULL);
}

typedef struct {
    HANDLE handle;
} mutex_t;

mutex_t mut_create() {
    mutex_t mut;
    
    mut.handle = CreateMutex(NULL, 0, NULL);
    
    return mut;
}

void mut_lock(mutex_t* mut) {
    WaitForSingleObject(mut->handle, WAIT_INFINITE);
}

void mut_unlock(mutex_t* mut) {
    ReleaseMutex(mut->handle);
}

s64 locked_add(volatile s64* addend, s64 val) {
    return InterlockedExchangeAdd64(addend, val);
}

thread_t thread_run(thread_func worker, void* data) {
    thread_t t; 
    t.handle = CreateThread(
                            NULL, 0,
                            worker, data,
                            0, 0);
    
    return t;
}

dword thread_wait_for_all(u32 tcount, const thread_t* threads, b32 for_all, dword ms) {
    return WaitForMultipleObjects(tcount, (const HANDLE*) threads, (BOOL) for_all, ms);
}

#endif