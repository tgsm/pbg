#ifndef ENGINE_WRAP_DKW_RGBA_H
#define ENGINE_WRAP_DKW_RGBA_H

#include "types.h"

class CDKW_RGBA {
public:
    CDKW_RGBA();
    CDKW_RGBA(const CDKW_RGBA& other);
    CDKW_RGBA(U8 r, U8 g, U8 b, U8 a);
    ~CDKW_RGBA() {}

    CDKW_RGBA operator*(const F32& scalar) const;
    void operator=(const CDKW_RGBA& other);

public:
    U8 m_r, m_g, m_b, m_a;
};
REQUIRE_SIZE(CDKW_RGBA, 0x4);

extern CDKW_RGBA DKW_RGBA_WHITE;
extern CDKW_RGBA DKW_RGBA_RED;
extern CDKW_RGBA DKW_RGBA_GREEN;
extern CDKW_RGBA DKW_RGBA_BLUE;
extern CDKW_RGBA DKW_RGBA_BLACK;
extern CDKW_RGBA DKW_RGBA_GREY;

#endif
