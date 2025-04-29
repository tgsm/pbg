#pragma once on

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"

class CEntityBhvTagBehavior : public CEntityBhvTagData {
private:
    u32 m_id;

public:
    CEntityBhvTagBehavior();
    virtual ~CEntityBhvTagBehavior();

    virtual void Set(CEntity* entity);

    virtual u32 GetType() {
        return BEHAVIOR_TAG_BEHAVIOR;
    }

    void Parse(DkXmd::CChunkIterator iter);
    CEntityBhvTagBehavior* GetBehavior(u32 id);
};
REQUIRE_SIZE(CEntityBhvTagBehavior, 0x10);
