#ifndef PIGLET_ENTITIES_CENTITYBHVTAGSENDMESSAGE_H
#define PIGLET_ENTITIES_CENTITYBHVTAGSENDMESSAGE_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "CMailBox.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagSendMessage : public CEntityBhvTagData {
private:
    std::string m_unkC;
    std::string m_unk10;
    u32 m_unk14; // unknown type

public:
    CEntityBhvTagSendMessage();
    virtual ~CEntityBhvTagSendMessage();

    virtual void Set(CEntity* entity) {
        entity->m_entity_manager->GetGame()->GetMailbox()->SendMessage(entity->m_unk0, m_unkC, m_unk10, m_unk14);
    }

    virtual u32 GetType() {
        return BEHAVIOR_TAG_SEND_MESSAGE;
    }
    
    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagSendMessage, 0x18);

#endif
