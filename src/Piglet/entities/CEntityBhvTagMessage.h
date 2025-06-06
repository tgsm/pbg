#ifndef PIGLET_ENTITIES_CENTITYBHVTAGMESSAGE_H
#define PIGLET_ENTITIES_CENTITYBHVTAGMESSAGE_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagMessage : public CEntityBhvTagData {
public:
    std::string m_unkC;
    std::string m_unk10;
    U32 m_unk14; // unknown type

public:
    CEntityBhvTagMessage();
    virtual ~CEntityBhvTagMessage();

    virtual void Set(CEntity* entity) {
        for (CEntityBhvTagData* bhv = m_unk4; bhv != NULL; bhv = bhv->m_unk4) {
            if (bhv->GetType() == BEHAVIOR_TAG_RESPONSE) {
                bhv->Set(entity);
            }
        }
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_MESSAGE;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagMessage, 0x18);

#endif
