#ifndef RWSDK_PLCORE_BAMEMORY_H
#define RWSDK_PLCORE_BAMEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwFreeList {
    int stub;
} RwFreeList; // Size unknown

RwFreeList* RwFreeListCreate(int a0, int a1, int a2);
int RwFreeListDestroy(RwFreeList* list);

#ifdef __cplusplus
}
#endif

#endif
