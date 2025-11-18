#ifndef PIGLET_ENTITIES_CENTITYBHVTAGCOLLISION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGCOLLISION_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagCollision : public CEntityBhvTagData {
private:
    BOOL m_on;

public:
    CEntityBhvTagCollision();
    virtual ~CEntityBhvTagCollision();

    void Parse(DkXmd::CChunkIterator iter);

    virtual void Set(CEntity* entity) {
        if (m_on == TRUE) {
            entity->AddFlag(ENTITY_FLAG_HAS_COLLISION);
        } else {
            entity->DelFlag(ENTITY_FLAG_HAS_COLLISION);
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_COLLISION;
    }
};
REQUIRE_SIZE(CEntityBhvTagCollision, 0x10);

#endif
