#ifndef RWA_CORE_RWAPARAM_H
#define RWA_CORE_RWAPARAM_H

#include <rwa/core/rwabase.h>
#include <rwa/core/rwaobj.h>
#include <rwa/core/rwauuid.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaParamType {
    RwaUniqueID uniqueID;
    void* rangeMin;
    void* rangeMax;
    RwaType type;
    struct RwaParamType* paramType;
    RwUInt32 flags;
    RwLLLink link;
} RwaParamType; // size: 0x28

typedef struct RwaParamTypeHandle {
    union {
        RwaUUID* paramTypeUUID;
        RwaParamType* paramType;
        RwaParamType* paramTypePtr;
    } u;
    RwBool isUUID;
} RwaParamTypeHandle; // size: 0x8

typedef RwaObj* (*RwaObjParamFunc)(); // FIXME: figure out params

typedef struct RwaInputParamDef {
    RwaUniqueID uniqueID;
    RwaParamTypeHandle paramHandle;
    RwaObjParamFunc setFunc;
} RwaInputParamDef; // size: 0x18

typedef struct RwaOutputParamDef {
    RwaUniqueID uniqueID;
    RwaParamTypeHandle paramHandle;
    RwaObjParamFunc getFunc;
} RwaOutputParamDef; // size: 0x18

typedef struct RwaInputParamMap {
    RwaObjParamFunc setFunc;
    RwUInt16 paramIndex;
    RwUInt8 pad[2];
} RwaInputParamMap; // size: 0x8

typedef struct RwaOutputParamMap {
    RwaObjParamFunc getFunc;
    RwUInt16 paramIndex;
    RwUInt8 pad[2];
} RwaOutputParamMap; // size: 0x8

RwaParamTypeHandle* _rwaParamTypeHandleRefreshType(RwaParamTypeHandle* handle);

#ifdef __cplusplus
}
#endif

#endif
