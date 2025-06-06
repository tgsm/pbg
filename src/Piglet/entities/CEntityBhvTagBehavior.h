#ifndef PIGLET_ENTITIES_CENTITYBHVTAGBEHAVIOR_H
#define PIGLET_ENTITIES_CENTITYBHVTAGBEHAVIOR_H

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"

class CEntityBhvTagBehavior : public CEntityBhvTagData {
private:
    U32 m_id;

public:
    CEntityBhvTagBehavior();
    virtual ~CEntityBhvTagBehavior();

    virtual void Set(CEntity* entity);

    virtual U32 GetType() {
        return BEHAVIOR_TAG_BEHAVIOR;
    }

    void Parse(DkXmd::CChunkIterator iter);
    CEntityBhvTagBehavior* GetBehavior(U32 id);
};
REQUIRE_SIZE(CEntityBhvTagBehavior, 0x10);

#endif
