#pragma once on

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

    virtual u32 GetType() {
        return BEHAVIOR_TAG_POSITION;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagPosition, 0x18);
