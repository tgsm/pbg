#ifndef PIGLET_ENTITIES_CENTITY3DSOUND_H
#define PIGLET_ENTITIES_CENTITY3DSOUND_H

#include "entities/CEntityObject.h"

class CEntity3DSound : public CEntityObject {
public:
    U8 m_unk40[0x34];

public:
    CEntity3DSound(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntity3DSound, 0x74);

#endif
