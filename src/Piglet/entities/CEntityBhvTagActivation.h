#ifndef PIGLET_ENTITIES_CENTITYBHVTAGACTIVATION_H
#define PIGLET_ENTITIES_CENTITYBHVTAGACTIVATION_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "entities/CEntityMesh.h"
#include "engine/xmd/CChunkIterator.h"

extern "C" {
extern int rand(void);
}

class CEntityBhvTagActivation : public CEntityBhvTagData {
private:
    BOOL m_on;

public:
    CEntityBhvTagActivation();
    virtual ~CEntityBhvTagActivation();

    virtual void Set(CEntity* entity) {
        if (m_on == TRUE) {
            entity->AddFlag(ENTITY_FLAG_ACTIVE);
            if (entity->GetType() > ENTITY_NPC23 && entity->GetType() < ENTITY_UNK36) {
                ((CEntityMesh*)entity)->UpdateAnimations(rand() / 32767.0f);
            }
        } else {
            entity->DelFlag(ENTITY_FLAG_ACTIVE);
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_ACTIVATION;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagActivation, 0x10);

#endif
