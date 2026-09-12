#ifndef RWA_CORE_RWAWAVEDEF_H
#define RWA_CORE_RWAWAVEDEF_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaWaveDef {
    char unk0;
} RwaWaveDef;

typedef RwaWaveDef* (*RwaWaveDefRegisterFunc)(void);

RwBool _rwaWaveDefRegister(RwaWaveDefRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs);

#ifdef __cplusplus
}
#endif

#endif
