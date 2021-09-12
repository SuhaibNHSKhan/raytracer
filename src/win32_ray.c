#include <windows.h>
#include "util.h"

typedef struct {
    HANDLE handle;
} thread_t;

s64 locked_add(volatile s64* addend, s64 val) {
    return InterlockedExchangeAdd64(addend, val);
}

thread_t thread_run(worker_thread worker, void* data) {
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