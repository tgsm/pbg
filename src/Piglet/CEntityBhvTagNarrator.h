#pragma once

#include <string>
#include "CEntityBhvTagData.h"
#include "CEntity.h"
#include "engine/CChunkIterator.h"

class CEntityBhvTagNarrator : public CEntityBhvTagData {
private:
    std::string m_narrator_line_id;

public:
    CEntityBhvTagNarrator();
    virtual ~CEntityBhvTagNarrator();

    void Parse(DkXmd::CChunkIterator iter);

    virtual void Set(CEntity* entity) {
        if (entity != NULL) {
            entity->m_entity_manager->GetGame()->PlayNarratorLine(m_narrator_line_id);
        }
    }

    virtual u32 GetType() {
        return 17;
    }
};
