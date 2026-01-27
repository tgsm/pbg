#ifndef ENGINE_WRAP_DKW_2DTOOLS_H
#define ENGINE_WRAP_DKW_2DTOOLS_H

#include <rwsdk/tool/2d/brush.h>
#include "engine/display/CTexture.h"
#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V2d.h"

class CDKW_2dBrush {
public:
    Rt2dBrush* m_rw_brush;

public:
    CDKW_2dBrush();
    ~CDKW_2dBrush();

    BOOL Create();
    BOOL SetRGBA(CDKW_RGBA*, CDKW_RGBA*, CDKW_RGBA*, CDKW_RGBA*);
    BOOL SetUV(CDKW_V2d*, CDKW_V2d*, CDKW_V2d*, CDKW_V2d*);
    BOOL SetTexture(CDKW_Texture*);
};
REQUIRE_SIZE(CDKW_2dBrush, 0x4);

#endif
