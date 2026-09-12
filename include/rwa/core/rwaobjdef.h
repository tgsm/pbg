#ifndef RWA_CORE_RWAOBJDEF_H
#define RWA_CORE_RWAOBJDEF_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaObjDef {
    RwChar unk0;
} RwaObjDef;

typedef RwaObjDef* (*RwaObjDefRegisterFunc)(void);

RwBool _rwaObjDefRegister(RwaObjDefRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs);

#ifdef __cplusplus
}
#endif

#endif
