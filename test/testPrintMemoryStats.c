#include "../include/heapAllocator.h"
#include <assert.h>

void testPrintMemoryStats()
{
    resetMemoryStats();
    void *ptr1 = heapAlloc(10);
    void *ptr2 = heapAlloc(20);

    heapFree(ptr1);
    heapFree(ptr2);

    printMemoryStats();
}