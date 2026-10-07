#ifndef RWA_CORE_RWAINTF_H
#define RWA_CORE_RWAINTF_H

#include <rwa/core/rwaobjinterface.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaObjHandle {
    RwaObj* obj;
    RwaObjInterface* objInterface;
    RwLLLink link;
    RwUInt32 flags;
    RwUInt32 refCount;
} RwaObjHandle;

// TODO
typedef struct UnkRwaInterfaceStruct {
    RwUInt32 unk0;
    RwUInt32 unk4;
    RwUInt32 unk8;
    UnkInterfaceFunc unkC;
    void* unk10;
    RwaObjHandle* unk14;
} UnkRwaInterfaceStruct;

// TODO
typedef struct UnkReleaseStruct {
    void* func0;
    void* unk4;
    RwaObjHandle* handle;
    RwaObjInterface* interface;
} UnkReleaseStruct;

RwBool _rwaObjHandleOpenModule(void);
void _rwaObjHandleCloseModule(void);
RwaObjHandle* RwaObjHandleCreateUsingInterface(struct RwaObjDef* objDef, RwaObjHandle* a1, RwUInt8* createParamsBuffer, RwaObjInterfaceDef* interfaceDef, UnkRwaInterfaceStruct* a4);
void RwaObjHandleRelease(RwaObjHandle* handle, UnkReleaseStruct* a1);
RwaObjHandle* RwaObjHandleSetParamData(RwaObjHandle* handle, RwInt32 inputID, RwInt32, void* data);
RwaObjHandle* RwaObjHandleSetParamInt32(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwInt32 data);
RwaObjHandle* RwaObjHandleSetParamUInt32(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwUInt32 data);
RwaObjHandle* RwaObjHandleSetParamReal(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwReal data);
RwaObjHandle* RwaObjHandleSetParamPointer(RwaObjHandle* handle, RwInt32 inputID, RwInt32, void* ptr);
RwInt32 RwaObjHandleGetParamInt32(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success);
RwUInt32 RwaObjHandleGetParamUInt32(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success);
void* RwaObjHandleGetParamPointer(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success);

#ifdef __cplusplus
}
#endif

#endif
