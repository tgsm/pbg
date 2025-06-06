#ifndef PIGLET_ENTITIES_CENTITYBHVTAGPOSITION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGPOSITION_H

#include "entities/CEntityBhvTagData.h"
#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"

class CEntityBhvTagPosition : public CEntityBhvTagData {
private:
    CDKW_V3d m_position;

public:
    CEntityBhvTagPosition();
    virtual ~CEntityBhvTagPosition();

    virtual void Set(CEntity* entity);

    virtual U32 GetType() {
        return BEHAVIOR_TAG_POSITION;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagPosition, 0x18);

#endif
