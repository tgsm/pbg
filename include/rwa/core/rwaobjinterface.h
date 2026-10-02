#ifndef RWA_CORE_RWAOBJINTERFACE_H
#define RWA_CORE_RWAOBJINTERFACE_H

#include <rwa/core/rwaparam.h>
#include <rwa/core/rwaobj.h>
#include <rwa/core/rwauuid.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RwaObjDef;

typedef struct RwaObjInterfaceDefParam {
    RwaUniqueID uniqueID;
    RwaParamTypeHandle paramType;
} RwaObjInterfaceDefParam; // size: 0x14

typedef void (*RwaObjInterfaceDefUnregFunc)(void);

typedef struct RwaObjInterfaceDef {
    RwaUniqueID uniqueID;
    RwaObjInterfaceDefParam* outputParams;
    RwaObjInterfaceDefParam* inputParams;
    RwUInt16 noInputParams;
    RwUInt16 noOutputParams;
    RwUInt32 flags;
    RwaObjInterfaceDefUnregFunc unregFunc;
    RwLLLink link;
} RwaObjInterfaceDef; // size: 0x28

typedef struct RwaObjInterface {
    RwaObjInterfaceDef* interfaceDef;
    RwaInputParamMap* inputParamMaps;
    RwaOutputParamMap* outputParamMaps;
    RwUInt32 refCount;
    RwUInt32 flags;
    RwLLLink link;
} RwaObjInterface; // size: 0x1C

typedef struct RwaObjDefInterfaceInfo {
    RwaObjInterfaceDef* defaultInterface;
    RwLinkList interfaceList;
    RwUInt16 noInputs;
    RwUInt16 noOutputs;
    RwaInputParamDef* inputParams;
    RwaOutputParamDef* outputParams;
} RwaObjDefInterfaceInfo; // size: 0x18

typedef RwaObjInterfaceDef* (*RwaObjInterfaceRegisterFunc)(void);
typedef RwaObjInterface* (*UnkInterfaceFunc)(RwUInt32 size, void* unk);

RwBool _rwaObjInterfaceOpenModule(void);
void _rwaObjInterfaceCloseModule(void);
RwBool _rwaObjInterfaceRegister(RwaObjInterfaceRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs);
RwaObjInterface* RwaObjInterfaceAttach(RwaObjInterfaceDef* def, struct RwaObjDef* objDef, UnkInterfaceFunc a2, void* a3);
RwBool RwaObjInterfaceRemove(RwaObjInterface* interface);
RwaObjInterfaceDef* _rwaObjInterfaceDefSupported(RwaObjInterfaceDef* def, struct RwaObjDef* objDef, RwInt32);
RwaObjInterfaceDef* RwaObjInterfaceDefSetup(RwaObjInterfaceDef* def, RwaObjInterfaceDefParam* inputParams, RwUInt32 noInputParams, RwaObjInterfaceDefParam* outputParams, RwUInt32 noOutputParams);
RwaObjInterfaceDef* RwaObjInterfaceDefAssignID(RwaObjInterfaceDef* def, const RwaUUID* uuid, const RwChar* name);
RwaObjInterfaceDef* RwaObjInterfaceDefCreate(RwaObjInterfaceDef* def);
RwBool RwaObjInterfaceDefDestroy(RwaObjInterfaceDef* def);
RwaObjInterfaceDef* RwaObjInterfaceDefFindByUUID(const RwaUUID* uuid);

#ifdef __cplusplus
}
#endif

#endif
