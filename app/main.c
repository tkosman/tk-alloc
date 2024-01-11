#include "heapAllocator.h"

int main(void)
{
    atexit(checkForUnfreedChunks);
    pthread_mutex_init(&global_malloc_lock, NULL);

    void *ptr = heapAlloc(5);
    void *ptr3 = heapAlloc(60);
    void *ptr2 = heapAlloc(5);
    heapFree(ptr3);
    void *ptr4 = heapAlloc(5);
    heapFree(ptr);
    heapFree(ptr2);
    heapFree(ptr4);
    fullDumpChunks();


    pthread_mutex_destroy(&global_malloc_lock);
    return 0;
}