#pragma once on

#include "types.h"

class CDKW_RGBAReal {
public:
    CDKW_RGBAReal();
    CDKW_RGBAReal(const CDKW_RGBAReal& other);
    CDKW_RGBAReal(float r, float g, float b, float a);
    ~CDKW_RGBAReal() {}

    void operator=(const CDKW_RGBAReal& other);
    CDKW_RGBAReal& operator*=(const f32& scalar); 

private:
    f32 m_r, m_g, m_b, m_a;
};
