#include "engine/wrap/DKW_Quat.h"

CDKW_Quat CDKW_Quat::ZERO;

CDKW_Quat::CDKW_Quat(const CDKW_Quat& other) {
    *this = other;
}

CDKW_Quat::CDKW_Quat(F32 a, F32 b, F32 c, F32 d) {
    m_d = d;
    m_abc.m_x = a;
    m_abc.m_y = b;
    m_abc.m_z = c;
}

CDKW_Quat::CDKW_Quat(const CDKW_V3d& abc, F32 d) {
    m_d = d;
    m_abc = abc;
}

CDKW_Quat CDKW_Quat::operator*(const CDKW_Quat& other) const {
    CDKW_Quat result;

    F32& result_a = result.m_abc.m_x; const F32& other_a = other.m_abc.m_x;
    F32& result_b = result.m_abc.m_y; const F32& other_b = other.m_abc.m_y;
    F32& result_c = result.m_abc.m_z; const F32& other_c = other.m_abc.m_z;
    F32& result_d = result.m_d; const F32& other_d = other.m_d;

    result_d = m_d * other_d - (m_abc.m_x * other_a + m_abc.m_y * other_b + m_abc.m_z * other_c);
    result_a = m_abc.m_y * other_c - m_abc.m_z * other_b;
    result_b = m_abc.m_z * other_a - m_abc.m_x * other_c;
    result_c = m_abc.m_x * other_b - m_abc.m_y * other_a;

    result_a += other_a * m_d;
    result_b += other_b * m_d;
    result_c += other_c * m_d;

    result_a += m_abc.m_x * other_d;
    result_b += m_abc.m_y * other_d;
    result_c += m_abc.m_z * other_d;

    return result;
}
