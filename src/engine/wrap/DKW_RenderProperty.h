#ifndef ENGINE_WRAP_DKW_RENDERPROPERTY_H
#define ENGINE_WRAP_DKW_RENDERPROPERTY_H

#include <rwsdk/batextur.h>
#include "types.h"
#include "wrap/DKW_RGBA.h"

#pragma push
#pragma pack(2)
class CDKW_RenderProperty {
public:
    CDKW_RenderProperty();
    ~CDKW_RenderProperty();

    CDKW_RenderProperty& operator=(const CDKW_RenderProperty& other);
    CDKW_RenderProperty Activate();

    void SetBorderColor(CDKW_RGBA color) {
        m_border_color = color;
        m_unk0 |= (1 << 5);
    }

    static CDKW_RenderProperty GetCurrentProperty();

public:
    U8 m_unk0;
    U32 m_unk2;
    U32 m_unk6;
    U32 m_unkA;
    RwTexture* m_rw_texture;
    U32 m_unk12;
    CDKW_RGBA m_border_color;
    U32 m_unk1A;
    U32 m_unk1E;
    U32 m_unk22;
};
REQUIRE_SIZE(CDKW_RenderProperty, 0x26);
#pragma pop

#endif
