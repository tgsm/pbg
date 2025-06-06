#include "wrap/DKW_V2d.h"

const CDKW_V2d CDKW_V2d::ZERO;
const CDKW_V2d CDKW_V2d::XAXIS(1.0f, 0.0f);
const CDKW_V2d CDKW_V2d::YAXIS(0.0f, 1.0f);
const CDKW_V2d CDKW_V2d::ONE(1.0f, 1.0f);

F32 CDKW_V2d::sm_Epsilon = 0.0000001f;

CDKW_V2d::CDKW_V2d() {
    m_x = m_y = 0.0f;
}

CDKW_V2d::CDKW_V2d(const CDKW_V2d& other) {
    m_x = other.m_x;
    m_y = other.m_y;
}

CDKW_V2d::CDKW_V2d(F32 x, F32 y) : m_x(x), m_y(y) {

}

CDKW_V2d CDKW_V2d::operator-(const CDKW_V2d& other) const {
    volatile CDKW_V2d result;
    F32 x = result.m_x = m_x - other.m_x;
    F32 y = result.m_y = m_y - other.m_y;
    return CDKW_V2d(x, y);
}

F32 CDKW_V2d::operator|(const CDKW_V2d& other) const {
    return m_x * other.m_x + m_y * other.m_y;
}

F32 CDKW_V2d::operator^(const CDKW_V2d& other) const {
    return m_x * other.m_y - m_y * other.m_x;
}

void CDKW_V2d::operator=(const CDKW_V2d& other) {
    m_x = other.m_x;
    m_y = other.m_y;
}
