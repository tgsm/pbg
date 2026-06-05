#ifndef RWSDK_PLCORE_BAMEMORY_H
#define RWSDK_PLCORE_BAMEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Where does this go?
typedef struct RwLLLink {
    struct RwLLLink* next;
    struct RwLLLink* prev;
} RwLLLink;
typedef struct RwLinkList {
    RwLLLink link;
} RwLinkList;

typedef struct RwFreeList {
    unsigned int entrySize;
    unsigned int entriesPerBlock;
    unsigned int heapSize;
    unsigned int alignment;
    RwLinkList blockList;
    unsigned int flags;
    RwLLLink link;
} RwFreeList; // size: 0x24

typedef void (*RwFreeListCallBack)(void*, void*);

typedef struct RwMemoryFunctions {
    void* (*rwmalloc)(unsigned long size); // 0x130
    void (*rwfree)(void* ptr);
    void* (*rwrealloc)(void* ptr, unsigned long size);
    void* (*rwcalloc)(unsigned long n, unsigned long size);
} RwMemoryFunctions; // size: 0x10

int _rwMemoryOpen(RwMemoryFunctions* memoryFuncs);
void _rwMemoryClose(void);
void _rwFreeListEnable(int);
void* _rwFreeListAllocReal(RwFreeList* freeList);
RwFreeList* _rwFreeListFreeReal(RwFreeList* freeList, void* ptr);
RwFreeList* RwFreeListCreate(unsigned int, unsigned int entriesPerBlock, unsigned int alignment);
RwFreeList* RwFreeListCreateAndPreallocateSpace(unsigned int, unsigned int entriesPerBlock, unsigned int alignment, unsigned int preallocBlocks, RwFreeList*);
int RwFreeListDestroy(RwFreeList* list);
RwFreeList* RwFreeListForAllUsed(RwFreeList* freeList, RwFreeListCallBack callback, void*);

#ifdef __cplusplus
}
#endif

#endif
