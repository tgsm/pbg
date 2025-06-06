#ifndef PIGLET_ENTITIES_CENTITYSCARYFACEBOX_H
#define PIGLET_ENTITIES_CENTITYSCARYFACEBOX_H

#include "entities/CEntityZone.h"

class CEntityScaryFaceBox : public CEntityZone {
public:
    U8 m_unk1BC[0x1E0 - 0x1BC];

public:
    CEntityScaryFaceBox(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityScaryFaceBox, 0x1E0);

#endif
