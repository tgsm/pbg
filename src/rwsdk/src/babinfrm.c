#include <rwsdk/babinfrm.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/babinary.h>
#include <rwsdk/plcore/baerr.h>

extern RwInt32 _rwPluginRegistryAddPluginStream(RwPluginRegistry* registry, RwUInt32 pluginID, RwPluginDataChunkReadCallBack readCB, RwPluginDataChunkWriteCallBack writeCB, RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwInt32 _rwPluginRegistryAddPlgnStrmlwysCB(RwPluginRegistry* registry, RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwUInt32 _rwPluginRegistryGetSize(RwPluginRegistry* registry, const void* ptr);

RwInt32 RwFrameRegisterPluginStream(RwUInt32 pluginID, RwPluginDataChunkReadCallBack readCB, RwPluginDataChunkWriteCallBack writeCB, RwPluginDataChunkGetSizeCallBack getSizeCB) {
    return _rwPluginRegistryAddPluginStream(&frameTKList, pluginID, readCB, writeCB, getSizeCB);
}

RwInt32 RwFrameSetStreamAlwaysCallBack(RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB) {
    return _rwPluginRegistryAddPlgnStrmlwysCB(&frameTKList, pluginID, alwaysCB);
}

// FIXME: Not sure if this function takes in a pointer to a frame list, or a triple pointer to a frame.
static RwBool rwFrameListMakeBreadthFirst(rwFrameList* a1, RwFrame* frame) {
    if (frame != NULL) {
        *(a1->frames) = frame;
        a1->frames++;
        rwFrameListMakeBreadthFirst(a1, frame->next);
        rwFrameListMakeBreadthFirst(a1, frame->child);
    }

    return TRUE;
}

rwFrameList* _rwFrameListInitialize(rwFrameList* list, RwFrame* frame) {
    RwInt32 count = RwFrameCount(frame);
    RwFrame** frames = RwMalloc(count * sizeof(RwFrame*));
    RwFrame** framesPtr;
    if (frames == NULL) {
        RwThrowErrorParams(1, E_RW_NOMEM, count * sizeof(RwFrame*));
        return NULL;
    }

    framesPtr = frames;
    rwFrameListMakeBreadthFirst((rwFrameList*)&framesPtr, frame);

    list->numFrames = count;
    list->frames = frames;
    return list;
}

RwBool _rwFrameListFindFrame(rwFrameList* list, RwFrame* frame, RwInt32* index) {
    int i;
    for (i = 0; i < list->numFrames; i++) {
        if (list->frames[i] == frame) {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;
}

rwFrameList* _rwFrameListDeinitialize(rwFrameList* list) {
    if (list->numFrames != 0) {
        RwFree(list->frames);
    }
    return list;
}

// rwStreamMatrix is close to this, but not exact.
typedef struct rwStreamMatrixAlmost {
    RwV3d right;
    RwV3d up;
    RwV3d at;
    RwV3d pos;
    int index;
    int mtxFlags;
} rwStreamMatrixAlmost; // size: 0x38

// FIXME: Figure out these magic constants.
RwUInt32 _rwFrameListStreamGetSize(rwFrameList* list) {
    RwUInt32 size = list->numFrames * sizeof(rwStreamMatrixAlmost) + 0x10;
    RwInt32 i;
    for (i = 0; i < list->numFrames; i++) {
        size += _rwPluginRegistryGetSize(&frameTKList, list->frames[i]) + 0xC;
    }

    return size;
}

extern RwPluginRegistry* _rwPluginRegistryReadDataChunks(RwPluginRegistry*, RwStream*, void*);
extern RwPluginRegistry* _rwPluginRegistryWriteDataChunks(RwPluginRegistry*, RwStream*, void*);

rwFrameList* _rwFrameListStreamWrite(rwFrameList* list, RwStream* stream) {
    RwUInt32 size = _rwFrameListStreamGetSize(list);
    RwUInt32 numFrames;
    RwInt32 i;

    if (!_rwStreamWriteVersionedChunkHeader(stream, 14, size, 0x34003, 0xFFFF)) {
        return NULL;
    }
    if (!_rwStreamWriteVersionedChunkHeader(stream, 1, list->numFrames * sizeof(rwStreamMatrixAlmost) + 0x4, 0x34003, 0xFFFF)) {
        return NULL;
    }

    numFrames = list->numFrames;
    RwMemLittleEndian32(&numFrames, sizeof(RwUInt32));
    if (!RwStreamWrite(stream, &numFrames, sizeof(RwUInt32))) {
        return NULL;
    }

    for (i = 0; i < list->numFrames; i++) {
        RwFrame* frame = list->frames[i];
        rwStreamMatrixAlmost unk;
        RwFrame* parent;
        unk.right = frame->modelling.right;
        unk.up = frame->modelling.up;
        unk.at = frame->modelling.at;
        unk.pos = frame->modelling.pos;
        parent = frame->object.parent;
        if (parent != NULL) {
            if (!_rwFrameListFindFrame(list, parent, &unk.index)) {
                return NULL;
            }
        } else {
            unk.index = -1;
        }
        RwMemRealToFloat32((RwReal*)&unk.right, sizeof(RwV3d));
        RwMemRealToFloat32((RwReal*)&unk.up, sizeof(RwV3d));
        RwMemRealToFloat32((RwReal*)&unk.at, sizeof(RwV3d));
        RwMemRealToFloat32((RwReal*)&unk.pos, sizeof(RwV3d));
        unk.mtxFlags = frame->modelling.flags;
        RwMemLittleEndian32((RwUInt32*)&unk, sizeof(unk));
        if (!RwStreamWrite(stream, &unk, sizeof(unk))) {
            return NULL;
        }
    }

    for (i = 0; i < list->numFrames; i++) {
        if (!_rwPluginRegistryWriteDataChunks(&frameTKList, stream, list->frames[i])) {
            return NULL;
        }
    }

    return list;
}

extern RwReal _rwMatrixNormalError(RwMatrix*);
extern RwReal _rwMatrixOrthogonalError(RwMatrix*);
extern RwReal _rwMatrixDeterminant(RwMatrix*);

rwFrameList* _rwFrameListStreamRead(RwStream* stream, rwFrameList* list) {
    RwInt32 numFrames;
    RwUInt32 length, version;
    RwInt32 i;
    rwStreamMatrixAlmost unk;

    if (!RwStreamFindChunk(stream, 1, &length, &version)) {
        return NULL;
    }

    if (version >= 0x31000 && version <= 0x34003) {
        if (RwStreamRead(stream, &numFrames, sizeof(numFrames)) != sizeof(numFrames)) {
            return NULL;
        }
        RwMemNative32((RwUInt32*)&numFrames, sizeof(numFrames));

        list->numFrames = numFrames;
        list->frames = RwMalloc(numFrames * sizeof(RwFrame*));
        if (list->frames == NULL) {
            RwThrowErrorParams(1, E_RW_NOMEM, numFrames * sizeof(RwFrame*));
            return NULL;
        }

        for (i = 0; i < numFrames; i++) {
            RwFrame* frame;
            RwMatrix* mtx;

            if (RwStreamRead(stream, &unk, sizeof(unk)) != sizeof(unk)) {
                RwFree(list->frames);
                return NULL;
            }

            RwMemNative32((RwUInt32*)&unk, sizeof(unk));
            RwMemFloat32ToReal((RwReal*)&unk.right, sizeof(RwV3d));
            RwMemFloat32ToReal((RwReal*)&unk.up, sizeof(RwV3d));
            RwMemFloat32ToReal((RwReal*)&unk.at, sizeof(RwV3d));
            RwMemFloat32ToReal((RwReal*)&unk.pos, sizeof(RwV3d));

            frame = RwFrameCreate();
            if (frame == NULL) {
                RwFree(list->frames);
                return NULL;
            }

            mtx = &frame->modelling;
            mtx->right = unk.right;
            mtx->up = unk.up;
            mtx->at = unk.at;
            mtx->pos = unk.pos;

            if (0.01f >= _rwMatrixNormalError(mtx) && 0.01f >= _rwMatrixOrthogonalError(mtx) && 0.99f <= _rwMatrixDeterminant(mtx)) {
                mtx->flags &= ~rwMATRIXINTERNALIDENTITY;
            } else {
                // Do we use RwMatrixType here?
                mtx->flags &= ~(rwMATRIXINTERNALIDENTITY | (1 << 1) | (1 << 0));
            }

            list->frames[i] = frame;
            if (unk.index >= 0) {
                RwFrameAddChild(list->frames[unk.index], frame);
            }
        }
    } else {
        RwThrowError(1, E_RW_BADVERSION);
        return NULL;
    }

    for (i = 0; i < numFrames; i++) {
        if (!_rwPluginRegistryReadDataChunks(&frameTKList, stream, list->frames[i])) {
            return NULL;
        }
    }

    return list;
}
