#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define CHUNK_SIZE sizeof(heapChunk) //? defining size of one chunk
#define ALIGNMENT 8 //? I'll align each chunk by 8 bytes
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) //? this is a macro for finding the next multiple of 8
#define MAGIC_NUMBER 0x12345678 //? used for identifying the chunk

//? this struct stores information about a single chunk
typedef struct heapChunk
{
    int magic;
    size_t size;
    bool isFree;
    heapChunk *next;

} heapChunk;

heapChunk *freeChunks = NULL;

heapChunk *findFreeChunk(heapChunk **last, size_t size)
{
    heapChunk *current = freeChunks;
    
    while(current && !(current->isFree && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}

void splitChunk(heapChunk *chunk, size_t size)
{
    heapChunk *newChunk = (heapChunk *)((char *)chunk + size + CHUNK_SIZE);
    newChunk->size = chunk->size - size - CHUNK_SIZE;
    newChunk->isFree = true;
    newChunk->next = chunk->next;
    newChunk->magic = MAGIC_NUMBER;

    chunk->size = size;
    chunk->next = newChunk;
}


void *heapAlloc(size_t size)
{
    if (size <= 0) return NULL;

    size_t totalSize;
    heapChunk *chunk, *last;

    //! Here mutex lock should be used

    size = ALIGN(size);
    totalSize = size + CHUNK_SIZE;

    if(freeChunks)
    {
        last = freeChunks;
        chunk = findFreeChunk(&last, size);
        if(chunk)
        {
            if(chunk->size - size >= CHUNK_SIZE + ALIGNMENT)
            {
                splitChunk(chunk, size);
            }
            chunk->isFree = false;
        }
    }

    if (!chunk)
    {
        chunk = sbrk(totalSize);
        if (chunk == (void*) -1)
        {
            //! Here mutex unlock should be used
            return NULL;
        }

        chunk->size = size;
        chunk->magic = MAGIC_NUMBER;
        chunk->next = NULL;
        chunk->isFree = false;

        if (last)
        {
            last->next = chunk;
        }
        else
        {
            freeChunks = chunk;
        }
    }

    //! Here mutex unlock should be used
    return (void*)(chunk + 1);
}

void heapFree(void* ptr)
{
    assert(false && "Not implemented yet");
}

void heapCollect()
{
    assert(false && "Not implemented yet");
}

int main(void)
{
    void *ptr = heapAlloc(5);
    printf("ptr: %p\n", ptr);

    void *ptr2 = heapAlloc(5);
    printf("ptr: %p\n", ptr2);

    void *ptr3 = heapAlloc(10);
    printf("ptr: %p\n", ptr3);
    return 0;
}