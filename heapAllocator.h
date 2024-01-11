#ifndef HEAP_ALLOCATOR_H
#define HEAP_ALLOCATOR_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h> 

#define CHUNK_SIZE sizeof(heapChunk) //? defining size of one chunk
#define ALIGNMENT 8 //? I'll align each chunk by 8 bytes
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) //? this is a macro for finding the next multiple of 8
#define MAGIC_NUMBER 0x12345678 //? used for identifying the chunk

#define heapAlloc(bytes) allocWithStats(bytes, extractFilename(__FILE__), __LINE__)

//? this struct stores information about a single chunk
typedef struct heapChunk
{
    int magic;
    size_t size;
    bool isFree;
    struct heapChunk *next;

    const char *file;
    int line;

} heapChunk;

typedef struct {
    int allocCalls;         
    size_t totalAllocated;   
    size_t peakMemory;
    size_t sbrkCalls;
    int corruptedChunks;
    int unfreedChunks;
} MemoryStats;

void heapFree(void *ptr);
void printMemoryStats();
void checkForUnfreedChunks();

#endif //HEAP_ALLOCATOR_H