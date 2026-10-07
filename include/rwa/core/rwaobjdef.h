#ifndef RWA_CORE_RWAOBJDEF_H
#define RWA_CORE_RWAOBJDEF_H

#include <rwa/core/rwaobjinterface.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaObjDef {
    RwChar unk0[0x20];
    RwLinkList waveDefList;
    RwChar unk28[0x40 - 0x28];
    RwUInt32 stateSizeAlign;
    RwChar unk44[0x4C - 0x44];
    RwaObjDefInterfaceInfo* interfaceInfo;
} RwaObjDef;

typedef RwaObjDef* (*RwaObjDefRegisterFunc)(void);
typedef RwBool (*RwaObjDefCallback)(RwaObjDef*, RwaObjDef*, void*); // FIXME: Figure out params

RwBool _rwaObjDefRegister(RwaObjDefRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs);
RwaObjDef* _rwaObjDefEnum(RwaObjDef*, RwaObjDef*, RwaObjDefCallback callback, void*, RwBool);

#ifdef __cplusplus
}
#endif

#endif
