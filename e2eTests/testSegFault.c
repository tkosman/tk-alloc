#include "../include/heapAllocator.h"

int main(void)
{
    void *ptr = (void*) -100;
    heapFree(ptr);
}