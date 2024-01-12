#include "heapAllocator.h"

MemoryStats mem_stats = {0};

//? for storing the all chunks
heapChunk *allChunks = NULL;

pthread_mutex_t global_malloc_lock;

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

void *sbrkWithStats(size_t size)
{
    mem_stats.sbrkCalls++;
    return sbrk((int)size);
}

void *myAlloc(size_t size, const char *file, int line)
{
    if (size <= 0) { return NULL; }

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
        chunk = sbrkWithStats(totalSize);

        if (errno == ENOMEM)
        {
            pthread_mutex_unlock(&global_malloc_lock);
            perror("sbrk failed");
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


    chunk->file = file;
    chunk->line = line;
    return (void*)(chunk + 1);
}


//? function extending heapAlloc to take file, line and stats
void *allocWithStats(size_t bytes, const char *file, int line)
{
    mem_stats.allocCalls++;
    mem_stats.totalAllocated += bytes;
    if (mem_stats.totalAllocated > mem_stats.peakMemory) {
        mem_stats.peakMemory = mem_stats.totalAllocated;
    }
    return myAlloc(bytes, file, line);
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
    if (!ptr)
    {
        printf("Freeing NULL pointer\n");
        return;
    }

    heapChunk *chunk = (heapChunk*)ptr - 1;

    if (chunk->isFree)
    {
        printf("Freeing already freed chunk\n");
        abort();
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);

    if(chunk->magic != MAGIC_NUMBER)
    {
        mem_stats.corruptedChunks++;
        pthread_mutex_unlock(&global_malloc_lock);
        perror("Memory corruption");
    }

    chunk->isFree = true;

    heapMergeChunks(allChunks);
    pthread_mutex_unlock(&global_malloc_lock);
}

const char* extractFilename(const char *path)
{
    const char *filename = path;
    for (const char *pth = path; *pth != '\0'; pth++)
    {
        if (*pth == '/' || *pth == '\\')
        {
            filename = pth + 1;
        }
    }
    return filename;
}

void checkForUnfreedChunks()
{
    heapChunk *current = allChunks;
    while (current)
    {
        if (!current->isFree)
        {
            mem_stats.unfreedChunks++;
            printf("Unfreed chunk: %p, Allocator: %s, Line: %d\n", current, current->file, current->line);
        }
        current = current->next;
    }
}

void fullDumpChunks()
{
    printf("Memory dump:\n");
    int chunkNumber = 0;
    heapChunk *current = allChunks;
    while (current != NULL)
    {
        printf("{CHUNK#%d start: %p end: %p, size: %zu, ", 
                chunkNumber++,
                (void*)current, 
                (void*)((char*)current + current->size + CHUNK_SIZE), 
                current->size);   
        
        if (!current->isFree) 
        {
            printf("allocated at FILE: %s, LINE: %d", 
                   current->file, 
                   current->line);
        } 
        else 
        {
            printf("free");
        }

        printf("}\n");
        current = current->next;
    }
}

void printMemoryStats()
{
    printf("Allocation calls: %d\n", mem_stats.allocCalls);
    printf("Allocated in total: %zu bites\n", mem_stats.totalAllocated);
    printf("Peek usage: %zu bites\n", mem_stats.peakMemory);
    printf("Calls of sbrk: %zu\n", mem_stats.sbrkCalls);
    printf("Corrupted chunks: %d\n", mem_stats.corruptedChunks);
    printf("Unfreed chunks: %d\n", mem_stats.unfreedChunks);
}

MemoryStats getMemoryStats()
{
    return mem_stats;
}

void resetMemoryStats()
{
    mem_stats.allocCalls = 0;
    mem_stats.totalAllocated = 0;
    mem_stats.peakMemory = 0;
    mem_stats.sbrkCalls = 0;
    mem_stats.corruptedChunks = 0;
    mem_stats.unfreedChunks = 0;
}

void *heapAlloc(size_t bytes)
{
    return heapAllocMacro(bytes);
}