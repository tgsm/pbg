#ifndef RWSDK_BAFRAME_H
#define RWSDK_BAFRAME_H

#include "rwsdk/plcore/bamatrix.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwFrame {
    char unk0[0x10];
    RwMatrix modelling;
    RwMatrix ltm;
    char unk90[0x8];
    RwFrame* child;
    RwFrame* next;
    RwFrame* root;
} RwFrame; // size: 0xA4 (probably right size?)

RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d*, int);
RwFrame* RwFrameRotate(float, RwFrame* frame, RwV3d*, int);

#ifdef __cplusplus
}
#endif

#endif
