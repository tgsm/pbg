#ifndef RWA_CORE_RWAFREELIST_H
#define RWA_CORE_RWAFREELIST_H

#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaFreeList {
    RwUInt32 entrySize;
    RwUInt32 entriesPerBlock;
    RwUInt32 heapSize;
    RwUInt32 alignment;
    RwLinkList blockList;
    RwUInt32 flags;
    RwLLLink link;
} RwaFreeList; // size: 0x24

typedef void (*RwaFreeListCallBack)(void*, void*);

RwaFreeList* RwaFreeListCreate(RwUInt32 a0, RwUInt32 entriesPerBlock, RwUInt32 alignment, RwUInt32 preallocBlocks, RwaFreeList* a4);
void* RwaFreeListAlloc(RwaFreeList* freeList);
void RwaFreeListDestroy(RwaFreeList* freeList);
void RwaFreeListFree(RwaFreeList* freeList, void* ptr);

#ifdef __cplusplus
}
#endif

#endif
