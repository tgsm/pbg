#ifndef PIGLET_ENTITIES_CENTITYBHVTAGNARRATOR_H
#define PIGLET_ENTITIES_CENTITYBHVTAGNARRATOR_H

#include <string>
#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

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
        return BEHAVIOR_TAG_NARRATOR;
    }
};
REQUIRE_SIZE(CEntityBhvTagNarrator, 0x10);

#endif
