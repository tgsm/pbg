#ifndef PIGLET_ENTITIES_CENTITYBHVTAGORIENTATION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGORIENTATION_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"

class CEntityBhvTagOrientation : public CEntityBhvTagData {
private:
    CDKW_V3d m_orientation;

public:
    CEntityBhvTagOrientation();
    virtual ~CEntityBhvTagOrientation();

    virtual u32 GetType() {
        return BEHAVIOR_TAG_ORIENTATION;
    }

    virtual void Set(CEntity* entity);

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagOrientation, 0x18);

#endif
