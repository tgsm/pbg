#include "engine/wrap/DKW_RenderProperty.h"
#include "rwsdk/batextur.h"

CDKW_RenderProperty::CDKW_RenderProperty() {
    m_unk0 = 0;
    m_unk2 = 0;
    m_unk6 = 0;
    m_unkA = 0;
    m_rw_texture = NULL;
    m_unk12 = 0;
    m_unk1A = 0;
    m_unk1E = 0;
    m_unk22 = 0;
}

CDKW_RenderProperty::~CDKW_RenderProperty() {
    if (m_rw_texture != NULL) {
        // is this supposed to not be freed?
        m_rw_texture = NULL;
    }
}

extern "C" void RwRenderStateGet(int, void*);
extern "C" void RwRenderStateSet(int, int);

// Incomplete
CDKW_RenderProperty CDKW_RenderProperty::GetCurrentProperty() {
    CDKW_RenderProperty ret;

    RwRaster* raster;
    RwRenderStateGet(1, raster);
    if (raster != NULL) {
        U32 local_40;
        RwTexture* texture = RwTextureCreate(raster);

        RwRenderStateGet(2, &local_40);
        texture->filterAddressing = (texture->filterAddressing & ~0xFF00) | ((local_40 << 8) & 0xF00) | (((local_40 & 0xF) << 12) & 0xF000);

        RwRenderStateGet(3, &local_40);
        texture->filterAddressing = (texture->filterAddressing & ~0xF00) | (local_40 & 0xF00);

        RwRenderStateGet(4, &local_40);
        texture->filterAddressing = (texture->filterAddressing & ~0xF000) | (local_40 & 0xF000);

        RwRenderStateGet(9, &local_40);
        texture->filterAddressing = (texture->filterAddressing & ~0xFF) | (local_40 & 0xFF);

        ret.m_rw_texture = texture;
        if (texture != NULL) {
            texture->refCount++;
            ret.m_unk0 |= (1 << 3);
        } else {
            ret.m_unk0 &= ~(1 << 3);
        }
    }

    U32 unk12;
    RwRenderStateGet(5, &unk12);
    ret.m_unk12 = unk12;
    ret.m_unk0 |= (1 << 4);

    int local_4c, local_50;
    RwRenderStateGet(6, &local_4c);
    RwRenderStateGet(8, &local_50);
    if (local_4c != 0 && local_50 != 0) {
        ret.m_unk2 = 3;
        ret.m_unk0 |= (1 << 0);
    } else if (local_4c != 0) {
        ret.m_unk2 = 1;
        ret.m_unk0 |= (1 << 0);
    } else if (local_50 != 0) {
        ret.m_unk2 = 2;
        ret.m_unk0 |= (1 << 0);
    } else {
        ret.m_unk2 = 0;
        ret.m_unk0 |= (1 << 0);
    }

    U32 unk6;
    RwRenderStateGet(7, &unk6);
    ret.m_unk6 = unk6;
    ret.m_unk0 |= (1 << 1);

    U32 unk1A, unk1E;
    RwRenderStateGet(10, &unk1A);
    RwRenderStateGet(11, &unk1E);
    ret.m_unk1A = unk1A;
    ret.m_unk1E = unk1E;
    ret.m_unk0 |= (1 << 6);

    U32 border_color;
    RwRenderStateGet(13, &border_color);
    ret.SetBorderColor(CDKW_RGBA((border_color >> 24) & 0xFF, (border_color >> 16) & 0xFF, (border_color >> 8) & 0xFF, (border_color >> 0) & 0xFF));

    U32 unk22;
    RwRenderStateGet(12, &unk22);
    ret.m_unk22 = unk22;
    ret.m_unk0 |= (1 << 7);

    return ret;
}

// Equivalent?: scheduling
CDKW_RenderProperty CDKW_RenderProperty::Activate() {
    CDKW_RenderProperty current = GetCurrentProperty();

    if (m_unk0 & (1 << 7)) {
        RwRenderStateSet(12, m_unk22);
    }

    if (m_unk0 & (1 << 5)) {
        RwRenderStateSet(13, m_border_color.alpha << 24 | m_border_color.red << 16 | m_border_color.green << 8 | m_border_color.blue << 0);
    }

    if (m_unk0 & (1 << 6)) {
        RwRenderStateSet(10, m_unk1A);
        RwRenderStateSet(11, m_unk1E);
    }

    if (m_unk0 & (1 << 1)) {
        RwRenderStateSet(7, m_unk6);
    }

    if (m_unk0 & (1 << 0)) {
        switch (m_unk2) {
            case 0:
                RwRenderStateSet(6, 0);
                RwRenderStateSet(8, 0);
                break;
            case 1:
                RwRenderStateSet(6, 1);
                RwRenderStateSet(8, 0);
                break;
            case 2:
                RwRenderStateSet(6, 0);
                RwRenderStateSet(8, 1);
                break;
            case 3:
                RwRenderStateSet(6, 1);
                RwRenderStateSet(8, 1);
                break;
        }
    }

    if (m_unk0 & (1 << 4)) {
        RwRenderStateSet(5, m_unk12);
    }

    if (m_unk0 & (1 << 3)) {
        RwRenderStateSet(1, (int)m_rw_texture->raster);

        const U32 unkF000 = (m_rw_texture->filterAddressing & 0xF000) >> 12;
        const U32 unkF00 = (m_rw_texture->filterAddressing & 0xF00) >> 8;
        U32 value = ((unkF00 == unkF000) ? unkF00 : 0);
        RwRenderStateSet(2, value);

        RwRenderStateSet(3, (m_rw_texture->filterAddressing & 0xF00) >> 8);
        RwRenderStateSet(4, (m_rw_texture->filterAddressing & 0xF000) >> 12);
        RwRenderStateSet(9, (m_rw_texture->filterAddressing & 0xFF) >> 0);
    } else {
        RwRenderStateSet(1, 0);
    }

    if (m_unk0 & (1 << 2)) {
        RwRenderStateSet(20, m_unkA);
    }

    return current;
}

CDKW_RenderProperty& CDKW_RenderProperty::operator=(const CDKW_RenderProperty& other) {
    this->m_unk0 = other.m_unk0;
    this->m_unk2 = other.m_unk2;
    this->m_unk6 = other.m_unk6;
    this->m_unkA = other.m_unkA;
    this->m_rw_texture = other.m_rw_texture;
    if (this->m_rw_texture != NULL) {
        this->m_rw_texture->refCount++;
    }
    this->m_unk12 = other.m_unk12;
    this->m_border_color = other.m_border_color;
    this->m_unk1A = other.m_unk1A;
    this->m_unk1E = other.m_unk1E;
    this->m_unk22 = other.m_unk22;

    return *this;
}
