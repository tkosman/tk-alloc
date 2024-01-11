#include "../include/heapAllocator.h"
#include <assert.h>

void testHeapFree()
{
    resetMemoryStats();
    void *ptr1 = heapAlloc(10);
    void *ptr2 = heapAlloc(20);

    heapFree(ptr1);
    heapFree(ptr2);

    MemoryStats stats = getMemoryStats();

    assert(stats.unfreedChunks == 0);
    assert(stats.totalAllocated == 30);
    assert(stats.allocCalls == 2);
}