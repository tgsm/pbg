#ifndef PIGLET_ENTITIES_CENTITYBHVTAGVISIBILITY_H
#define PIGLET_ENTITIES_CENTITYBHVTAGVISIBILITY_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagVisibility : public CEntityBhvTagData {
private:
    BOOL m_on;

public:
    CEntityBhvTagVisibility();
    virtual ~CEntityBhvTagVisibility();

    virtual void Set(CEntity* entity) {
        if (m_on == TRUE) {
            entity->AddFlag(1 << 1);
        } else {
            entity->DelFlag(1 << 1);
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_VISIBILITY;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagVisibility, 0x10);

#endif
