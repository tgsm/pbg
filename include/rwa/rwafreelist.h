#ifndef RWA_RWAFREELIST_H
#define RWA_RWAFREELIST_H

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Where does this go?
typedef struct RwaLLLink {
    struct RwaLLLink* next;
    struct RwaLLLink* prev;
} RwaLLLink;
typedef struct RwaLinkList {
    RwaLLLink link;
} RwaLinkList;

typedef struct RwaFreeList {
    unsigned int entrySize;
    unsigned int entriesPerBlock;
    unsigned int heapSize;
    unsigned int alignment;
    RwaLinkList blockList;
    unsigned int flags;
    RwaLLLink link;
} RwaFreeList; // size: 0x24

typedef void (*RwaFreeListCallBack)(void*, void*);

RwaFreeList* RwaFreeListCreate(unsigned int a0, unsigned int entriesPerBlock, unsigned int alignment, unsigned int preallocBlocks, RwaFreeList* a4);
void RwaFreeListDestroy(RwaFreeList* freeList);
void RwaFreeListFree(RwaFreeList* freeList, void* ptr);

#ifdef __cplusplus
}
#endif

#endif
