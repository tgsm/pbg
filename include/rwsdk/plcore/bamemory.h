#ifndef RWSDK_PLCORE_BAMEMORY_H
#define RWSDK_PLCORE_BAMEMORY_H

#include <rwsdk/rwtypes.h>

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
    RwUInt32 entrySize;
    RwUInt32 entriesPerBlock;
    RwUInt32 heapSize;
    RwUInt32 alignment;
    RwLinkList blockList;
    RwUInt32 flags;
    RwLLLink link;
} RwFreeList; // size: 0x24

typedef void (*RwFreeListCallBack)(void*, void*);

typedef struct RwMemoryFunctions {
    void* (*rwmalloc)(unsigned long size); // 0x130
    void (*rwfree)(void* ptr);
    void* (*rwrealloc)(void* ptr, unsigned long size);
    void* (*rwcalloc)(unsigned long n, unsigned long size);
} RwMemoryFunctions; // size: 0x10

#define RwMalloc(size) RwEngineInstance->memoryFuncs.rwmalloc(size)
#define RwFree(ptr) RwEngineInstance->memoryFuncs.rwfree(ptr)
#define RwRealloc(ptr, size) RwEngineInstance->memoryFuncs.rwrealloc(ptr, size)
#define RwCalloc(n, size) RwEngineInstance->memoryFuncs.rwcalloc(n, size)

RwBool _rwMemoryOpen(RwMemoryFunctions* memoryFuncs);
void _rwMemoryClose(void);
void _rwFreeListEnable(RwBool);
void* _rwFreeListAllocReal(RwFreeList* freeList);
RwFreeList* _rwFreeListFreeReal(RwFreeList* freeList, void* ptr);
RwFreeList* RwFreeListCreate(RwUInt32, RwUInt32 entriesPerBlock, RwUInt32 alignment);
RwFreeList* RwFreeListCreateAndPreallocateSpace(RwUInt32, RwUInt32 entriesPerBlock, RwUInt32 alignment, RwUInt32 preallocBlocks, RwFreeList*);
RwBool RwFreeListDestroy(RwFreeList* list);
RwFreeList* RwFreeListForAllUsed(RwFreeList* freeList, RwFreeListCallBack callback, void*);

#ifdef __cplusplus
}
#endif

#endif
