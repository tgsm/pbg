#include "wrap/DKW_RGBA.h"

CDKW_RGBA DKW_RGBA_WHITE = CDKW_RGBA(0xFF, 0xFF, 0xFF, 0xFF);
CDKW_RGBA DKW_RGBA_RED = CDKW_RGBA(0xFF, 0x00, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_GREEN = CDKW_RGBA(0x00, 0xFF, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_BLUE = CDKW_RGBA(0x00, 0x00, 0xFF, 0xFF);
CDKW_RGBA DKW_RGBA_BLACK = CDKW_RGBA(0x00, 0x00, 0x00, 0xFF);
CDKW_RGBA DKW_RGBA_GREY = CDKW_RGBA(0x7F, 0x7F, 0x7F, 0xFF);

CDKW_RGBA::CDKW_RGBA() {
    red = 0x7F;
    green = 0x7F;
    blue = 0x7F;
    alpha = 0xFF;
}

CDKW_RGBA::CDKW_RGBA(const CDKW_RGBA& other) {
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
}

CDKW_RGBA::CDKW_RGBA(U8 r, U8 g, U8 b, U8 a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

CDKW_RGBA CDKW_RGBA::operator*(const F32& scalar) const {
    volatile CDKW_RGBA result;
    U8 r = result.red = red * scalar;
    U8 g = result.green = green * scalar;
    U8 b = result.blue = blue * scalar;
    U8 a = result.alpha = alpha * scalar;
    return CDKW_RGBA((U8)r, (U8)g, (U8)b, (U8)a);
}

void CDKW_RGBA::operator=(const CDKW_RGBA& other) {
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
}
