#ifndef ENGINE_DISPLAY_IFRAMABLE_H
#define ENGINE_DISPLAY_IFRAMABLE_H

#include "engine/wrap/DKW_Frame.h"
#include "engine/wrap/DKW_Matrix.h"

// Used for UnkTRTInline
#pragma push
#pragma supress_warnings on

namespace DKDSP {

class IFramable {
private:
    U8 m_unk0[4];

public:
    IFramable();

    virtual CDKW_Frame* GetFrame() = 0;
    virtual void SetFrame(CDKW_Frame* frame) = 0;

    inline CDKW_V3d GetPosition() {
        return GetFrame()->GetPosition();
    }

    inline CDKW_V3d GetAt() {
        return (CDKW_V3d&)(GetFrame()->m_rwframe->modelling.at);
    }

    inline void LookAtInline(CDKW_V3d vec1, CDKW_V3d vec2, CDKW_V3d vec3) {
        ((CDKW_Matrix*)(&GetFrame()->m_rwframe->modelling))->LookAt(vec1, vec2, vec3);
        RwFrameUpdateObjects(GetFrame()->m_rwframe);
    }

    inline void LookAtInline(CDKW_V3d vec1, CDKW_V3d vec2) {
        ((CDKW_Matrix*)(&GetFrame()->m_rwframe->modelling))->LookAt((CDKW_V3d&)RwFrameGetLTM(GetFrame()->m_rwframe)->pos, vec1, vec2);
        RwFrameUpdateObjects(GetFrame()->m_rwframe);
    }

    inline void UpdateModelPos(CDKW_V3d pos) {
        RwMatrix* model = &GetFrame()->m_rwframe->modelling;
        model->pos = pos;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(GetFrame()->m_rwframe);
    }

    inline void UpdateModelPosRef(CDKW_V3d& pos) {
        RwMatrix* model = &GetFrame()->m_rwframe->modelling;
        model->pos = pos;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(GetFrame()->m_rwframe);
    }

    inline void UnkTRTInline(CDKW_V3d* a1, const F32& rot, int a2) {
        CDKW_V3d pos = GetPosition();
        GetFrame()->Translate(reinterpret_cast<CDKW_V3d&>(pos.Negated()), 2);
        GetFrame()->Rotate(a1, rot, a2);
        GetFrame()->Translate(pos, 2);
    }

    inline void UnkTRTInline2(CDKW_V3d* a1, const F32& rot, int a2) {
        GetFrame()->UnkTRTInline(a1, rot, a2);
    }
};

}

#pragma pop

#endif
