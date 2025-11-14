#ifndef ENGINE_PHYSICS_DKPH_PRIMITIVES_H
#define ENGINE_PHYSICS_DKPH_PRIMITIVES_H

#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V3d.h"

namespace DkPh {

class BVolume {
public:
    virtual ~BVolume();
    void SetCenter(CDKW_V3d& center);

    void Draw(CDKW_RGBA, CDKW_V3d, CDKW_V3d) const {}

public:
    CDKW_V3d m_center;
    U8 m_unk10[0x1C - 0x10];
    U32* m_unk1C; // Unknown type
};
REQUIRE_SIZE(BVolume, 0x20);

class Dynamics;

struct RBody {
    RBody() : unk0(NULL), dynamics(NULL), unk8(0.0f, 0.0f, 0.0f), unk14(0.0f, 0.0f, 0.0f) {

    }

    ~RBody();

    BVolume* unk0;
    Dynamics* dynamics;
    CDKW_V3d unk8;
    CDKW_V3d unk14;
};

}

#endif
