#ifndef PIGLET_ENTITIES_CENTITYNPCDOOR_H
#define PIGLET_ENTITIES_CENTITYNPCDOOR_H

#include "entities/CEntityNPC.h"

class CEntityNPCDoor : public CEntityNPC {
private:
    std::string m_bad_grimace_str;
    std::string m_victory_str;
    U32 m_needed_grimace;

public:
    CEntityNPCDoor(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCDoor();

    virtual U32 GetType() {
        return ENTITY_NPC_DOOR;
    }
    virtual void Reset();
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateSpecialBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);

    BOOL CanLaunchFightMode();

    void SendVictoryStartMessage() {
        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_victory_str, "START", 0);
        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unk0, "DIE", 0);
    }

    void SendBadGrimaceStartMessage() {
        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_bad_grimace_str, "START", 0);
    }
};
REQUIRE_SIZE(CEntityNPCDoor, 0x2E0);

#endif
