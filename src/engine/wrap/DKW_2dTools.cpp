#include "engine/wrap/DKW_2dTools.h"

CDKW_2dBrush::CDKW_2dBrush() {
    m_rw_brush = NULL;
}

CDKW_2dBrush::~CDKW_2dBrush() {
    if (m_rw_brush != NULL) {
        Rt2dBrushDestroy(m_rw_brush);
    }
}

BOOL CDKW_2dBrush::Create() {
    Rt2dBrush* brush = Rt2dBrushCreate();
    m_rw_brush = brush;
    return brush != NULL;
}

BOOL CDKW_2dBrush::SetRGBA(CDKW_RGBA* a1, CDKW_RGBA* a2, CDKW_RGBA* a3, CDKW_RGBA* a4) {
    return Rt2dBrushSetRGBA(m_rw_brush, a1, a2, a3, a4) != NULL;
}

BOOL CDKW_2dBrush::SetUV(CDKW_V2d* a1, CDKW_V2d* a2, CDKW_V2d* a3, CDKW_V2d* a4) {
    return Rt2dBrushSetUV(m_rw_brush, a1, a2, a3, a4) != NULL;
}

BOOL CDKW_2dBrush::SetTexture(CDKW_Texture* texture) {
    return Rt2dBrushSetTexture(m_rw_brush, texture) != NULL;
}
