#ifndef RWA_CORE_RWASYMBOLTABLE_H
#define RWA_CORE_RWASYMBOLTABLE_H

#include <rwa/core/rwaparam.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

// Fabricated. Ghost Rider doesn't have this in its STABS.
typedef struct RwaSymbolTable {
    RwInt32 noParamTypes;
    RwLinkList paramTypeList;
} RwaSymbolTable; // size: 0xC

RwaSymbolTable* _rwaSymbolTableInit(RwaSymbolTable* table);
RwaSymbolTable* _rwaSymbolTableDestroy(RwaSymbolTable* table);
RwaSymbolTable* _rwaSymbolTableAddParamType(RwaSymbolTable* table, RwaParamType* type);
RwaSymbolTable* _rwaSymbolTableRemoveParamType(RwaSymbolTable* table, RwaParamType* type);
RwaParamType* _rwaSymbolTableFindParamTypeByUUID(RwaSymbolTable* table, RwaUUID* uuid);
RwaParamType* _rwaSymbolTableFindParamTypeFirstDepend(RwaSymbolTable* table, RwaParamType* type);
RwaParamType* _rwaSymbolTableFindParamTypeByIndex(RwaSymbolTable* table, RwInt32 index);

#ifdef __cplusplus
}
#endif

#endif
