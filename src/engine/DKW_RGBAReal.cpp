#include "DKW_RGBAReal.h"

CDKW_RGBAReal DKW_RGBAREAL_WHITE = CDKW_RGBAReal(1.0f, 1.0f, 1.0f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_RED = CDKW_RGBAReal(1.0f, 0.0f, 0.0f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_GREEN = CDKW_RGBAReal(0.0f, 1.0f, 0.0f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_BLUE = CDKW_RGBAReal(0.0f, 0.0f, 1.0f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_BLACK = CDKW_RGBAReal(0.0f, 0.0f, 0.0f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_GREY = CDKW_RGBAReal(0.5f, 0.5f, 0.5f, 1.0f);
CDKW_RGBAReal DKW_RGBAREAL_FORCEBLACK = CDKW_RGBAReal(-1.0f, -1.0f, -1.0f, 1.0f);

CDKW_RGBAReal::CDKW_RGBAReal() {
    m_r = 0.5f;
    m_g = 0.5f;
    m_b = 0.5f;
    m_a = 1.0f;
}

CDKW_RGBAReal::CDKW_RGBAReal(const CDKW_RGBAReal& other) {
    m_r = other.m_r;
    m_g = other.m_g;
    m_b = other.m_b;
    m_a = other.m_a;
}

CDKW_RGBAReal::CDKW_RGBAReal(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {

}

void CDKW_RGBAReal::operator=(const CDKW_RGBAReal& other) {
    m_r = other.m_r;
    m_g = other.m_g;
    m_b = other.m_b;
    m_a = other.m_a;
}

CDKW_RGBAReal& CDKW_RGBAReal::operator*=(const f32& scalar) {
    m_r *= scalar;
    m_g *= scalar;
    m_b *= scalar;
    m_a *= scalar;
    return *this;
}
