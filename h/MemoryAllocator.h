//
// Created by os on 7/24/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H
#include "../lib/hw.h"

struct FreeMem
{
    FreeMem *next;
    //FreeMem *prev;
    size_t size;
};

class MemoryAllocator
{
public:
    static MemoryAllocator& getInstance()
    {
        static MemoryAllocator instance;
        return instance;
    }
    static FreeMem *freeMemHead;
    static FreeMem *allocatedHead;

    static void *mem_alloc(size_t size);
    static int mem_free(void* address);
private:
    MemoryAllocator(){}
    MemoryAllocator(MemoryAllocator const&);
    void operator = (MemoryAllocator const&);
};
#endif //PROJECT_BASE_MEMORYALLOCATOR_H
