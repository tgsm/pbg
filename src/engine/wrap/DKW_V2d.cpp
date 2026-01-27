#include "wrap/DKW_V2d.h"

const CDKW_V2d CDKW_V2d::ZERO;
const CDKW_V2d CDKW_V2d::XAXIS(1.0f, 0.0f);
const CDKW_V2d CDKW_V2d::YAXIS(0.0f, 1.0f);
const CDKW_V2d CDKW_V2d::ONE(1.0f, 1.0f);

F32 CDKW_V2d::sm_Epsilon = 0.0000001f;

CDKW_V2d::CDKW_V2d() {
    x = y = 0.0f;
}

CDKW_V2d::CDKW_V2d(const CDKW_V2d& other) {
    x = other.x;
    y = other.y;
}

CDKW_V2d::CDKW_V2d(F32 x_, F32 y_) {
    x = x_;
    y = y_;
}

CDKW_V2d CDKW_V2d::operator-(const CDKW_V2d& other) const {
    volatile CDKW_V2d result;
    F32 x_ = result.x = x - other.x;
    F32 y_ = result.y = y - other.y;
    return CDKW_V2d(x_, y_);
}

F32 CDKW_V2d::operator|(const CDKW_V2d& other) const {
    return x * other.x + y * other.y;
}

F32 CDKW_V2d::operator^(const CDKW_V2d& other) const {
    return x * other.y - y * other.x;
}

void CDKW_V2d::operator=(const CDKW_V2d& other) {
    x = other.x;
    y = other.y;
}
