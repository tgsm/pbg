#ifndef PIGLET_ENTITIES_CENTITYESCAPINGOBJECT_H
#define PIGLET_ENTITIES_CENTITYESCAPINGOBJECT_H

#include "entities/CEntityMesh.h"

class CEntityEscapingObject : public CEntityMesh {
public:
    U8 m_unkF4[0x1B0 - 0xF4];

public:
    CEntityEscapingObject(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityEscapingObject, 0x1B0);

#endif
