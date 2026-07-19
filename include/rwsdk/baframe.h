#ifndef RWSDK_BAFRAME_H
#define RWSDK_BAFRAME_H

#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bastream.h>
#include <rwsdk/plcore/batkreg.h>
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

typedef RwFrame* (*RwFrameCallBack)(RwFrame*, void*);

void* _rwFrameOpen(void*, RwInt32 offset, RwInt32);
void* _rwFrameClose(void*, RwInt32, RwInt32);
RwFrame* _rwFrameCloneAndLinkClones(RwFrame* frame);
RwFrame* _rwFramePurgeClone(RwFrame* frame);
RwBool RwFrameDirty(RwFrame* frame);
RwFrame* RwFrameCreate(void);
RwBool RwFrameDestroy(RwFrame* frame);
RwBool RwFrameDestroyHierarchy(RwFrame* frame);
RwFrame* RwFrameUpdateObjects(RwFrame* frame);
RwMatrix* RwFrameGetLTM(RwFrame* frame);
RwFrame* RwFrameAddChild(RwFrame* frame, RwFrame* child);
RwFrame* RwFrameRemoveChild(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callback, void*);
RwInt32 RwFrameCount(RwFrame* frame);
RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d*, RwInt32);
RwFrame* RwFrameScale(RwFrame* frame, RwV3d*, RwInt32 scale);
RwFrame* RwFrameTransform(RwFrame* frame, RwMatrix*, RwInt32);
RwFrame* RwFrameRotate(RwFrame* frame, RwV3d*, float, RwInt32);
RwInt32 RwFrameRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);

#ifdef __cplusplus
}
#endif

#endif
