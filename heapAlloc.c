#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define CHUNK_SIZE sizeof(heapChunk) //? defining size of one chunk
#define ALIGNMENT 8 //? I'll align each chunk by 8 bytes
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) //? this is a macro for finding the next multiple of 8
#define MAGIC_NUMBER 0x12345678 //? used for identifying the chunk

pthread_mutex_t global_malloc_lock;

//? this struct stores information about a single chunk
typedef struct heapChunk
{
    int magic;
    size_t size;
    bool isFree;
    struct heapChunk *next;

} heapChunk;

//? for storing the all chunks
heapChunk *allChunks = NULL;

//? simple linear search for finding a next free chunk
heapChunk *findFreeChunk(heapChunk **last, size_t size)
{
    heapChunk *current = allChunks;
    
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
    heapChunk *chunk = NULL;
    heapChunk *last = NULL;

    pthread_mutex_lock(&global_malloc_lock);

    size = ALIGN(size);
    totalSize = size + CHUNK_SIZE;

    if(allChunks)
    {
        last = allChunks;
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
            pthread_mutex_unlock(&global_malloc_lock);
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
            allChunks = chunk;
        }
    }

    pthread_mutex_unlock(&global_malloc_lock);

    return (void*)(chunk + 1);
}

void heapMergeChunks(heapChunk *chunk)
{
    while (chunk->next && chunk->next->isFree)
    {
        chunk->size += CHUNK_SIZE + chunk->next->size;
        chunk->next = chunk->next->next;
    }
}

void heapFree(void* ptr)
{
    assert(false && "Add mutexes");

    if (!ptr) return;

    heapChunk *chunk = (heapChunk*)ptr - 1;

    if(chunk->magic != MAGIC_NUMBER)
    {
        perror("Memory corruption");
    }

    chunk->isFree = true;
    if (chunk->next)
    {
        heapMergeChunks(chunk);
    }
}


void printAll() {
    heapChunk *current = allChunks;

    while (current != NULL) {
        printf("Chunk at address: %p, Size: %zu, Free: %s\n", 
               (void *)current, 
               current->size, 
               current->isFree ? "Yes" : "No");

        current = current->next;
    }
}

int main(void)
{
    void *ptr = heapAlloc(5);
    void *ptr2 = heapAlloc(5);
    void *ptr3 = heapAlloc(10);
    printAll();
    return 0;
}