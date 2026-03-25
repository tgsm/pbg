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

RwFreeList* RwFreeListCreate(int a0, int a1, int a2);
RwFreeList* RwFreeListCreateAndPreallocateSpace(int, unsigned int entriesPerBlock, unsigned int alignment, int preallocBlocks, RwFreeList*);
int RwFreeListDestroy(RwFreeList* list);

#ifdef __cplusplus
}
#endif

#endif
