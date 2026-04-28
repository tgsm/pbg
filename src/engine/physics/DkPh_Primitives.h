#ifndef ENGINE_PHYSICS_DKPH_PRIMITIVES_H
#define ENGINE_PHYSICS_DKPH_PRIMITIVES_H

#include "engine/wrap/DKW_Quat.h"
#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V3d.h"

namespace DkPh {

struct RBody;

class BVolume {
public:
    BVolume() {
        m_unk1C = NULL;
        m_unk20 = 0;
        m_unk24 = -1;
    }
    virtual ~BVolume();

    virtual void funcC();
    virtual void func10();
    virtual void func14();
    virtual void func18();
    virtual void func1C();
    virtual void SetQuat(CDKW_Quat& quat);

    void SetCenter(CDKW_V3d& center);

    void Draw(CDKW_RGBA, CDKW_V3d, CDKW_V3d) const {}

public:
    CDKW_V3d m_center;
    CDKW_V3d m_unk10;
    RBody* m_unk1C;
    int m_unk20;
    int m_unk24;
};
REQUIRE_SIZE(BVolume, 0x28);

class BVolumeAABB : public BVolume {

};

// TODO
class BVolumeOBB : public BVolumeAABB {
public:
    BVolumeOBB() {
        m_unk30 = 0.0f;
        m_unk2C = 0.0f;
        m_unk28 = 0.0f;
        m_unk34 = 1.0f;
    }

    F32 m_unk28;
    F32 m_unk2C;
    F32 m_unk30;
    F32 m_unk34;
};

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
