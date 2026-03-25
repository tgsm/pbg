#ifndef RWSDK_BAFRAME_H
#define RWSDK_BAFRAME_H

#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bastream.h>
#include <rwsdk/batypehf.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwFrame {
    RwObject object;
    RwLLLink inDirtyListLink;
    RwMatrix modelling;
    RwMatrix ltm;
    RwLinkList objectList;
    struct RwFrame* child;
    struct RwFrame* next;
    struct RwFrame* root;
} RwFrame; // size: 0xA4

typedef void (*RwFrameDataConstructorCB)(void*, int, int);
typedef void* (*RwFrameDataDestructorCB)(void*, int, int);
typedef void* (*RwFrameDataCopierCB)(void*, const void*, int, int);
typedef RwFrame* (*RwFrameCallBack)(RwFrame*, void*);

int RwFrameDirty(RwFrame* frame);
RwFrame* RwFrameCreate(void);
int RwFrameDestroy(RwFrame* frame);
RwFrame* RwFrameUpdateObjects(RwFrame* frame);
RwMatrix* RwFrameGetLTM(RwFrame* frame);
RwFrame* RwFrameAddChild(RwFrame* frame, RwFrame* child);
RwFrame* RwFrameRemoveChild(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callback, void*);
int RwFrameCount(RwFrame* frame);
RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d*, int);
RwFrame* RwFrameScale(RwFrame* frame, RwV3d*, int scale);
RwFrame* RwFrameTransform(RwFrame* frame, RwMatrix*, int);
RwFrame* RwFrameRotate(RwFrame* frame, RwV3d*, float, int);
int RwFrameRegisterPlugin(int a0, int a1, RwFrameDataConstructorCB constructorCB, RwFrameDataDestructorCB destructorCB, RwFrameDataCopierCB copierCB);

#ifdef __cplusplus
}
#endif

#endif
