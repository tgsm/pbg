#include "engine/wrap/DKW_Quat.h"

CDKW_Quat CDKW_Quat::ZERO;

CDKW_Quat::CDKW_Quat(const CDKW_Quat& other) {
    *this = other;
}

CDKW_Quat::CDKW_Quat(F32 a, F32 b, F32 c, F32 d) {
    real = d;
    imag.x = a;
    imag.y = b;
    imag.z = c;
}

CDKW_Quat::CDKW_Quat(const CDKW_V3d& abc, F32 d) {
    real = d;
    // FIXME: Make CDKW_V3d inherit RwV3d
    imag = *(RwV3d*)&abc;
}

CDKW_Quat CDKW_Quat::operator*(const CDKW_Quat& other) const {
    CDKW_Quat result;

    F32& result_x = result.imag.x;
    F32& result_y = result.imag.y;
    F32& result_z = result.imag.z;
    F32& result_r = result.real;

    result_r = real * other.real - (imag.x * other.imag.x + imag.y * other.imag.y + imag.z * other.imag.z);
    result_x = imag.y * other.imag.z - imag.z * other.imag.y;
    result_y = imag.z * other.imag.x - imag.x * other.imag.z;
    result_z = imag.x * other.imag.y - imag.y * other.imag.x;

    result_x += other.imag.x * real;
    result_y += other.imag.y * real;
    result_z += other.imag.z * real;

    result_x += imag.x * other.real;
    result_y += imag.y * other.real;
    result_z += imag.z * other.real;

    return result;
}
