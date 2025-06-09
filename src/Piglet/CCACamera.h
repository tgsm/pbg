#ifndef PIGLET_CCACAMERA_H
#define PIGLET_CCACAMERA_H

#include <vector>
#include "CCAZone.h"

class CCACamera {
public:
    std::vector<CCAZone*> GetZoneList();
    void SetActiveSystemForAZone(char* zone, char* system);
};

#endif
