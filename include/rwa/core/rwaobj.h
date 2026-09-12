#ifndef RWA_CORE_RWAOBJ_H
#define RWA_CORE_RWAOBJ_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaObj {
    RwUInt8 unk0[0x3C];
    RwInt32 unk3C;
} RwaObj; // Unknown size

RwaObj* RwaObjGetTopLevelObject(void);
RwaObj* RwaObjStartExecute(RwaObj*, RwBool);
RwaObj* RwaObjFinishExecute(RwaObj*, RwBool);

#ifdef __cplusplus
}
#endif

#endif
