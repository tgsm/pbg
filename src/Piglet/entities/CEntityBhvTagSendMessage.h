#ifndef PIGLET_ENTITIES_CENTITYBHVTAGSENDMESSAGE_H
#define PIGLET_ENTITIES_CENTITYBHVTAGSENDMESSAGE_H

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "CMailBox.h"
#include "CGame.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagSendMessage : public CEntityBhvTagData {
private:
    std::string m_receiver;
    std::string m_type;
    U32 m_param;

public:
    CEntityBhvTagSendMessage();
    virtual ~CEntityBhvTagSendMessage();

    virtual void Set(CEntity* entity) {
        entity->GetManager()->GetGame()->GetMailbox()->SendMessage(entity->GetName(), m_receiver, m_type, m_param);
    }

    virtual U32 GetType() {
        return BEHAVIOR_TAG_SEND_MESSAGE;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagSendMessage, 0x18);

#endif
