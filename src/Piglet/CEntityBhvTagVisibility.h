#pragma once

#include "CEntityBhvTagData.h"
#include "CEntity.h"
#include "engine/CChunkIterator.h"

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

    virtual u32 GetType() {
        return 7;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
