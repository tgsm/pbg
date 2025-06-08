#ifndef PIGLET_ENTITIES_CENTITYWORLD_H
#define PIGLET_ENTITIES_CENTITYWORLD_H

#include "engine/display/CClump.h"
#include "entities/CEntityObject.h"

class CEntityWorld : public CEntityObject {
public:
    DKDSP::CClump* m_clump;
    U8 m_unk44[0xC];

public:
    CEntityWorld(CEntityManager* entity_manager, std::string name);

    virtual DKDSP::CClump* GetClump() { return m_clump; }
};
REQUIRE_SIZE(CEntityWorld, 0x50);

#endif
