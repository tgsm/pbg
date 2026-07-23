#ifndef RWSDK_PLCORE_BALIST_H
#define RWSDK_PLCORE_BALIST_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwSList {
    RwUInt8* listElements;
    RwInt32 numElementsFilled;
    RwInt32 numElementsAlloced;
    RwInt32 entrySize;
} RwSList; // size: 0x10

RwSList* _rwSListCreate(RwInt32 entrySize);
void _rwSListEmpty(RwSList* list);
RwUInt8* _rwSListGetNewEntry(RwSList* list);
RwUInt8* _rwSListGetNewEntries(RwSList* list, int numEntries);
RwUInt8* _rwSListGetArray(RwSList* list);
RwBool _rwSListDestroy(RwSList* list);
RwInt32 _rwSListGetNumEntries(RwSList* list);
RwUInt8* _rwSListToArray(RwSList* list);
RwUInt8* _rwSListGetEntry(RwSList* list, int entryNum);

#ifdef __cplusplus
}
#endif

#endif
