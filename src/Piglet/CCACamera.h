#ifndef PIGLET_CCACAMERA_H
#define PIGLET_CCACAMERA_H

#include <vector>
#include "engine/wrap/DKW_V3d.h"
#include "CCAZone.h"

class CCACamera {
public:
    CDKW_V3d m_position;
    CDKW_V3d m_target;
    U8 m_unk18[0x1C - 0x18];
    F32 m_roll;
    U8 m_unk20[0x90 - 0x20];

public:
    struct UnkUpdateResult {
        U8 unk[32];
    };

    UnkUpdateResult Update(CDKW_V3d&, F32 dt);
    CDKW_V3d GetCameraPosition();
    CDKW_V3d GetCameraTarget();
    std::vector<CCAZone*> GetZoneList();
    F32 GetCameraRoll();
    void SetActiveSystemForAZone(char* zone, char* system);
};
REQUIRE_SIZE(CCACamera, 0x90);

#endif
