#ifndef RWA_CORE_RWAOBJINTERFACE_H
#define RWA_CORE_RWAOBJINTERFACE_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaObjInterfaceDef {
    RwChar unk0;
} RwaObjInterfaceDef;

typedef RwaObjInterfaceDef* (*RwaObjInterfaceRegisterFunc)(void);

RwBool _rwaObjInterfaceRegister(RwaObjInterfaceRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs);

#ifdef __cplusplus
}
#endif

#endif
