#include "engine/wrap/DKW_Frame.h"
#include "rwsdk/baframe.h"
#include "rwsdk/plcore/bamemory.h"
#include "wrap/DKW_InstanceEngine.h"
#include "wrap/DKW_Matrix.h"

// FIXME: Make this file look better.

int CDKW_Frame::m_LocalOffset = -1;
int CDKW_Frame::ms_NbObject = 0;
RwFreeList* CDKW_Frame::ms_pMemEntry = NULL;

CDKW_Frame* CDKW_Frame::GetInstance(RwFrame* rw_frame) {
    return *(CDKW_Frame**)((int)rw_frame + m_LocalOffset);
}

void CDKW_Frame::DataConstructor(void* a0, int a1, int a2) {
    RwFrame* frame = (RwFrame*)a0;
    if (CDKW_Frame::m_LocalOffset > 0) {
        *(CDKW_Frame**)((int)frame + m_LocalOffset) = NULL;
        *(U32*)((int)frame + m_LocalOffset + 4) = 0; // ?
    }
}

void* CDKW_Frame::DataDestructor(void* a0, int a1, int a2) {
    RwFrame* frame = (RwFrame*)a0;
    if (CDKW_Frame::m_LocalOffset > 0) {
        CDKW_Frame* wrap_frame = *(CDKW_Frame**)((int)frame + m_LocalOffset);
        if (wrap_frame != NULL) {
            wrap_frame->m_rwframe = NULL;
            delete wrap_frame;
            *(CDKW_Frame**)((int)frame + m_LocalOffset) = NULL;
        }
    }

    return frame;
}

void* CDKW_Frame::DataCopier(void* dst, const void* src, int, int) {
    U32 unk4 = *(U32*)((int)src + m_LocalOffset + 4);
    *(CDKW_Frame**)((int)dst + m_LocalOffset) = CDKW_InstanceEngine::ms_pInstanceEngine->NewFrame(dst, unk4);
    *(U32*)((int)dst + m_LocalOffset + 4) = unk4;
    return dst;
}

BOOL CDKW_Frame::FrameReadStream(void* ptr, int, int) {
    *(CDKW_Frame**)((int)ptr + m_LocalOffset) = CDKW_InstanceEngine::ms_pInstanceEngine->NewFrame(ptr, 0);
    *(U32*)((int)ptr + m_LocalOffset + 4) = 0;
    return TRUE;
}

extern "C" {
extern RwStream* RwStreamReadInt32(RwStream* stream, void*, int);
extern RwStream* RwStreamWriteInt32(RwStream* stream, void*, int);
}

struct DataForStream {
    U32 unk0;
};

static inline BOOL ReadSucceeded(RwStream* stream, DataForStream* data, int a2) {
    return RwStreamReadInt32(stream, data, a2) == NULL ? 0 : 1;
}

RwStream* CDKW_Frame::DataReadStream(RwStream* stream, int a1, void* a2, int a3, int a4) {
    DataForStream data;
    if (!ReadSucceeded(stream, &data, 1)) {
        return NULL;
    } else {
        *(U32*)((int)a2 + m_LocalOffset + 4) = data.unk0;
    }
    return stream;
}

static inline BOOL WriteSucceeded(RwStream* stream, DataForStream* data, int a2) {
    return RwStreamWriteInt32(stream, data, a2) == NULL ? 0 : 1;
}

RwStream* CDKW_Frame::DataWriteStream(RwStream* stream, int a1, const void* a2, int a3, int a4) {
    DataForStream data;
    data.unk0 = *(U32*)((int)a2 + m_LocalOffset + 4);
    return (!WriteSucceeded(stream, &data, 1)) ? NULL : stream;
}

int CDKW_Frame::DataGetStreamSize(const void*, int, int) {
    return 8;
}

CDKW_Frame::CDKW_Frame() {
    m_rwframe = RwFrameCreate();
    *(CDKW_Frame**)((int)m_rwframe + m_LocalOffset) = this;
    RwFrameTransform(GetRwFrame(), (RwMatrix*)&CDKW_Matrix::IDENTITY, 0);
    m_unk8 = 0;
}

CDKW_Frame::CDKW_Frame(RwFrame* rw_frame) {
    m_rwframe = rw_frame;
    m_unk8 = 1;
}

CDKW_Frame::~CDKW_Frame() {
    if (m_unk8 == 1) {
        if (m_rwframe != NULL) {
            *(CDKW_Frame**)((int)m_rwframe + m_LocalOffset) = NULL;
            RwFrameDestroy(m_rwframe);
            m_rwframe = NULL;
        }
    }

    // custom delete operator called here for this
}

extern "C" {
typedef RwStream* (*RwFrameDataReadStreamCB)(RwStream*, int, void*, int, int);
typedef RwStream* (*RwFrameDataWriteStreamCB)(RwStream*, int, const void*, int, int);
typedef int (*RwFrameDataGetStreamSizeCB)(const void*, int, int);
typedef BOOL (*RwFrameReadStreamCB)(void*, int, int);
extern int RwFrameRegisterPluginStream(int, RwFrameDataReadStreamCB readCB, RwFrameDataWriteStreamCB writeCB, RwFrameDataGetStreamSizeCB getSizeCB);
extern int RwFrameSetStreamAlwaysCallBack(int, RwFrameReadStreamCB readCB);
}

BOOL CDKW_Frame::PluginAttach() {
    ms_pMemEntry = RwFreeListCreate(12, 100, 16);
    m_LocalOffset = RwFrameRegisterPlugin(8, 0x200000, DataConstructor, DataDestructor, DataCopier);
    if (m_LocalOffset < 0) {
        return FALSE;
    }
    int unkResult = RwFrameRegisterPluginStream(0x200000, DataReadStream, DataWriteStream, DataGetStreamSize);
    if (unkResult != m_LocalOffset) {
        return FALSE;
    }
    unkResult = RwFrameSetStreamAlwaysCallBack(0x200000, FrameReadStream);
    if (unkResult != m_LocalOffset) {
        return FALSE;
    }
    return TRUE;
}

BOOL CDKW_Frame::PluginDetach() {
    RwFreeListDestroy(ms_pMemEntry);
    return TRUE;
}

struct UnkFrameCBData {
    void* (*callback)(void*, int);
    int unk4;
};

static int DefaultFrameCB(RwFrame* frame, void* data) {
    UnkFrameCBData cbdata = *(UnkFrameCBData*)data;
    void* result = cbdata.callback(*(CDKW_Frame**)((int)frame + CDKW_Frame::m_LocalOffset), cbdata.unk4);
    if (result != NULL) {
        return *(int*)((void*)((int)result + 4));
    } else {
        return 0;
    }
}

struct FACData {
    CDKW_Frame::FrameCallback callback;
    void* unk4;
};

void CDKW_Frame::ManageForAllChildren(FrameCallback callback, void* a2) {
    FACData data;
    data.callback = callback;
    data.unk4 = a2;
    RwFrameForAllChildren(m_rwframe, DefaultFrameCB, &data);
}
