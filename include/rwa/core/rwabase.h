#ifndef RWA_CORE_RWABASE_H
#define RWA_CORE_RWABASE_H

#include <rwsdk/rwtypes.h>
#include <rwsdk/badevice.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef RwBool (*RwaCoreCodeModuleOpenFunc)(void);
typedef void (*RwaCoreCodeModuleCloseFunc)(void);

typedef struct RwaCoreCodeModule {
    RwaCoreCodeModuleOpenFunc openFunc;
    RwaCoreCodeModuleCloseFunc closeFunc;
} RwaCoreCodeModule;

typedef enum RwaType {
    rwaTYPE_UINT8 = (1 << 0),
    rwaTYPE_INT8 = (1 << 1),
    rwaTYPE_UINT16 = (1 << 2),
    rwaTYPE_INT16 = (1 << 3),
    rwaTYPE_UINT32 = (1 << 4),
    rwaTYPE_INT32 = (1 << 5),
    rwaTYPE_UINT64 = (1 << 6),
    rwaTYPE_INT64 = (1 << 7),
    rwaTYPE_UINT128 = (1 << 8),
    rwaTYPE_INT128 = (1 << 9),
    rwaTYPE_FLOAT = (1 << 10),
    rwaTYPE_DOUBLE = (1 << 11),
    rwaTYPE_POINTER = (1 << 12),
    rwaTYPE_ARRAY = (1 << 13),
    rwaTYPE_ALIAS = (1 << 14),
} RwaType;

RwBool _rwaCoreOpen(RwMemoryFunctions* memFuncs, RwBool usingRwG);
void RwaCoreClose(void);
void RwaCoreSuspend(void);
void RwaCoreResume(void);
void RwaCoreUpdateStart(void);
void RwaCoreUpdateFinish(void);
RwBool RwaCoreRegisterDefaultObjects(void);
RwUInt32 RwaSetBlockAlign(RwUInt32 align);
RwUInt32 RwaSizeOf(RwaType type);

#ifdef __cplusplus
}
#endif

#endif
