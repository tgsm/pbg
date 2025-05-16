#ifndef ENGINE_WRAP_DKW_RGBA_H
#define ENGINE_WRAP_DKW_RGBA_H

#include "types.h"

class CDKW_RGBA {
public:
    CDKW_RGBA();
    CDKW_RGBA(const CDKW_RGBA& other);
    CDKW_RGBA(u8 r, u8 g, u8 b, u8 a);
    ~CDKW_RGBA() {}

    CDKW_RGBA operator*(const f32& scalar) const;
    void operator=(const CDKW_RGBA& other);

public:
    u8 m_r, m_g, m_b, m_a;
};
REQUIRE_SIZE(CDKW_RGBA, 0x4);

#endif
