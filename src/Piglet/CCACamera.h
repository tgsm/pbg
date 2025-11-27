#ifndef PIGLET_CCACAMERA_H
#define PIGLET_CCACAMERA_H

#include <vector>
#include "engine/wrap/DKW_V3d.h"
#include "CCAZone.h"

class CCACamera {
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

#endif
