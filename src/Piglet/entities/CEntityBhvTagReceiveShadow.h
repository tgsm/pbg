#ifndef PIGLET_ENTITIES_CENTITYBHVTAGRECEIVESHADOW_H
#define PIGLET_ENTITIES_CENTITYBHVTAGRECEIVESHADOW_H

#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityBhvTagData.h"
#include "entities/CEntityMesh.h"
#include "CGamePartIngame.h"

class CEntityBhvTagReceiveShadow : public CEntityBhvTagData {
private:
    BOOL m_on;

public:
    CEntityBhvTagReceiveShadow();
    virtual ~CEntityBhvTagReceiveShadow();

    virtual void Set(CEntity* entity) {
        if (m_on == TRUE) {
            ((CEntityMesh*)entity)->m_unk44 = TRUE;
        } else {
            ((CEntityMesh*)entity)->m_unk44 = FALSE;
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_RECEIVE_SHADOW;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagReceiveShadow, 0x10);

#endif
