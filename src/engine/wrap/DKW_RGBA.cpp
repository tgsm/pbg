#include "wrap/DKW_RGBA.h"

CDKW_RGBA DKW_RGBA_WHITE = CDKW_RGBA(0xFF, 0xFF, 0xFF, 0xFF);
CDKW_RGBA DKW_RGBA_RED = CDKW_RGBA(0xFF, 0x00, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_GREEN = CDKW_RGBA(0x00, 0xFF, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_BLUE = CDKW_RGBA(0x00, 0x00, 0xFF, 0xFF);
CDKW_RGBA DKW_RGBA_BLACK = CDKW_RGBA(0x00, 0x00, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_GREY = CDKW_RGBA(0x7F, 0x7F, 0x7F, 0xFF);

CDKW_RGBA::CDKW_RGBA() {
    m_r = 0x7F;
    m_g = 0x7F;
    m_b = 0x7F;
    m_a = 0xFF;
}

CDKW_RGBA::CDKW_RGBA(const CDKW_RGBA& other) {
    m_r = other.m_r;
    m_g = other.m_g;
    m_b = other.m_b;
    m_a = other.m_a;
}

CDKW_RGBA::CDKW_RGBA(u8 r, u8 g, u8 b, u8 a) : m_r(r), m_g(g), m_b(b), m_a(a) {

}

CDKW_RGBA CDKW_RGBA::operator*(const f32& scalar) const {
    volatile CDKW_RGBA result;
    u8 r = result.m_r = m_r * scalar;
    u8 g = result.m_g = m_g * scalar;
    u8 b = result.m_b = m_b * scalar;
    u8 a = result.m_a = m_a * scalar;
    return CDKW_RGBA((u8)r, (u8)g, (u8)b, (u8)a);
}

void CDKW_RGBA::operator=(const CDKW_RGBA& other) {
    m_r = other.m_r;
    m_g = other.m_g;
    m_b = other.m_b;
    m_a = other.m_a;
}
