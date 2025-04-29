#pragma once on

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
            entity->AddFlag(1 << 2);
        } else {
            entity->DelFlag(1 << 2);
        }
    }

    virtual u32 GetType() {
        return BEHAVIOR_TAG_COLLISION;
    }
};
REQUIRE_SIZE(CEntityBhvTagCollision, 0x10);
