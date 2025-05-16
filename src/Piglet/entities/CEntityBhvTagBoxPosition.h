#ifndef PIGLET_ENTITIES_CENTITYBHVTAGBOXPOSITION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGBOXPOSITION_H

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"

class CEntityBhvTagBoxPosition : public CEntityBhvTagData {
private:
    int m_x;
    int m_y;

public:
    CEntityBhvTagBoxPosition();
    virtual ~CEntityBhvTagBoxPosition();

    virtual void Set(CEntity* entity);

    virtual u32 GetType() {
        return BEHAVIOR_TAG_BOX_POSITION;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagBoxPosition, 0x14);

#endif
