#ifndef RWA_CORE_RWAOBJ_H
#define RWA_CORE_RWAOBJ_H

#include <rwsdk/rwtypes.h>

struct RwaObjDef;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaObj {
    struct RwaObjDef* definition;
    RwUInt8 unk4[0x20 - 0x4];
    void* state;
    RwUInt8 unk24[0x28 - 0x24];
} RwaObj; // size: 0x28

RwaObj* RwaObjCreate(struct RwaObjDef* objDef, RwaObj* a1, RwInt32 a2, RwUInt8* createParamsBuffer, RwInt32 a4, RwInt32 a5);
void RwaObjDestroy(RwaObj* obj, void* unkCallback, void*);
RwaObj* RwaObjGetTopLevelObject(void);
RwaObj* RwaObjStartExecute(RwaObj*, RwBool);
RwaObj* RwaObjFinishExecute(RwaObj*, RwBool);

#ifdef __cplusplus
}
#endif

#endif
