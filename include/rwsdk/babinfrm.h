#ifndef RWSDK_BABINFRM_H
#define RWSDK_BABINFRM_H

#include <rwsdk/baframe.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rwFrameList {
    RwFrame** frames;
    RwInt32 numFrames;
} rwFrameList; // size: 0x8

RwInt32 RwFrameRegisterPluginStream(RwUInt32 pluginID, RwPluginDataChunkReadCallBack readCB, RwPluginDataChunkWriteCallBack writeCB, RwPluginDataChunkGetSizeCallBack getSizeCB);
RwInt32 RwFrameSetStreamAlwaysCallBack(RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB);
rwFrameList* _rwFrameListInitialize(rwFrameList* list, RwFrame* frame);
RwBool _rwFrameListFindFrame(rwFrameList* list, RwFrame* frame, RwInt32* index);
rwFrameList* _rwFrameListDeinitialize(rwFrameList* list);
RwUInt32 _rwFrameListStreamGetSize(rwFrameList* list);
rwFrameList* _rwFrameListStreamWrite(rwFrameList* list, RwStream* stream);
rwFrameList* _rwFrameListStreamRead(RwStream* stream, rwFrameList* list);

#ifdef __cplusplus
}
#endif

#endif
