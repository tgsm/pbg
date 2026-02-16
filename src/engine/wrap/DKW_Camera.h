#ifndef ENGINE_WRAP_DKW_CAMERA_H
#define ENGINE_WRAP_DKW_CAMERA_H

#include <rwsdk/bacamera.h>
#include "engine/wrap/DKW_Frame.h"
#include "engine/wrap/DKW_Node3d.h"

// FIXME: This needs a home
class CDKW_AlphaSortedAtomicList {
public:
    static void Render(CDKW_AlphaSortedAtomicList*);
    static U32 ms_nNumberOfEntries;
};

// TODO
struct SDKW_Frustum {
    F32 unk0;
    F32 unk4;
    F32 unk8;
    F32 unkC;
    F32 unk10;
    F32 unk14;
};

// TODO
class CDKW_Camera : public CDKW_Node3d {
public:
    RwCamera* m_rw_camera;
    U8 m_unk8[0x1C - 0x8];
    CDKW_Frame* m_wrap_frame;

    static int ms_NbObject;
    static RwFreeList* ms_pMemEntry;
    static CDKW_AlphaSortedAtomicList* ms_pAlphaAtomic;

public:
    CDKW_Camera();
    virtual ~CDKW_Camera();

    virtual RwFrame* GetRwFrame() { return (RwFrame*)m_rw_camera->object.object.parent; }

    static void FlushAlphaAtomic() {
        if (ms_pAlphaAtomic != NULL) {
            ms_pAlphaAtomic->Render(ms_pAlphaAtomic);
            ms_pAlphaAtomic = NULL;
        }
    }

    void SetFrustum(SDKW_Frustum frustum);
    SDKW_Frustum GetFrustum();
    void SetFOV(F32, F32);
    F32 GetFOV();
    F32 GetAspectRatio();
};
// Unknown size

#endif
