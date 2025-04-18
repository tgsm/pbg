#pragma once on

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/CChunkIterator.h"
#include "engine/DKW_V3d.h"

class CEntityBhvTagOrientation : public CEntityBhvTagData {
private:
    CDKW_V3d m_orientation;

public:
    CEntityBhvTagOrientation();
    virtual ~CEntityBhvTagOrientation();

    virtual u32 GetType() {
        return 3;
    }

    virtual void Set(CEntity* entity);

    void Parse(DkXmd::CChunkIterator iter);
};
