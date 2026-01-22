#ifndef RWSDK_BAFRAME_H
#define RWSDK_BAFRAME_H

#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bastream.h>

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

typedef void (*RwFrameDataConstructorCB)(void*, int, int);
typedef void* (*RwFrameDataDestructorCB)(void*, int, int);
typedef void* (*RwFrameDataCopierCB)(void*, const void*, int, int);
typedef RwFrame* (*RwFrameForAllChildrenCB)(RwFrame*, void*);

RwFrame* RwFrameCreate(void);
int RwFrameDestroy(RwFrame* frame);
RwFrame* RwFrameUpdateObjects(RwFrame* frame);
RwMatrix* RwFrameGetLTM(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, void* callback, void*);
RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d*, int);
RwFrame* RwFrameScale(RwFrame* frame, RwV3d*, int scale);
RwFrame* RwFrameTransform(RwFrame* frame, RwMatrix*, int);
RwFrame* RwFrameRotate(RwFrame* frame, RwV3d*, float, int);
int RwFrameRegisterPlugin(int a0, int a1, RwFrameDataConstructorCB constructorCB, RwFrameDataDestructorCB destructorCB, RwFrameDataCopierCB copierCB);

#ifdef __cplusplus
}
#endif

#endif
