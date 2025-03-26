#pragma once

#include "CEntityBhvTagData.h"
#include "CEntity.h"
#include "engine/CChunkIterator.h"

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
        return 8;
    }
};
