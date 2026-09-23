#ifndef RWA_CORE_RWALLIST_H
#define RWA_CORE_RWALLIST_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaLLNode {
    struct RwaLLNode* prev;
    struct RwaLLNode* next;
    void* data;
} RwaLLNode; // size: 0xC

RwBool _rwaLListModuleOpen(void);
void _rwaLListModuleClose(void);
RwaLLNode* RwaLListAddData(RwaLLNode* list, void* data);
RwaLLNode* RwaLListRemoveData(RwaLLNode* list, void* data);
RwInt32 RwaLListFindDataIndex(RwaLLNode* list, void* data);
void RwaLListEmpty(RwaLLNode* list);

#ifdef __cplusplus
}
#endif

#endif
