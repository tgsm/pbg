#pragma once on

#include "entities/CEntityBhvTagData.h"
#include "engine/CChunkIterator.h"
#include "engine/DKW_V3d.h"

class CEntityBhvTagPosition : public CEntityBhvTagData {
private:
    CDKW_V3d m_position;

public:
    CEntityBhvTagPosition();
    virtual ~CEntityBhvTagPosition();

    virtual void Set(CEntity* entity);

    virtual u32 GetType() {
        return 2;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagPosition, 0x18);
