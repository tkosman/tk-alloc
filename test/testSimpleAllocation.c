#include "../include/heapAllocator.h"
#include <assert.h>

void testHeapAlloc()
{
    resetMemoryStats();
    void *ptr1 = heapAlloc(10);
    void *ptr2 = heapAlloc(20);

    MemoryStats stats = getMemoryStats();

    assert(stats.allocCalls == 2);
    assert(stats.totalAllocated == 30);

    heapFree(ptr1);
    heapFree(ptr2);
}