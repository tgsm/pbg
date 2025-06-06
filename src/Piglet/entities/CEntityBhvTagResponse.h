#ifndef PIGLET_ENTITIES_CENTITYBHVTAGRESPONSE_H
#define PIGLET_ENTITIES_CENTITYBHVTAGRESPONSE_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagResponse : public CEntityBhvTagData {
public:
    CEntityBhvTagResponse();
    virtual ~CEntityBhvTagResponse();

    virtual void Set(CEntity* entity) {
        for (CEntityBhvTagData* bhv = m_unk4; bhv != NULL; bhv = bhv->m_unk0) {
            bhv->Set(entity);
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_RESPONSE;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagResponse, 0xC);

#endif
