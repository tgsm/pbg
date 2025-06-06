#ifndef PIGLET_ENTITIES_CENTITYNPCDOOR_H
#define PIGLET_ENTITIES_CENTITYNPCDOOR_H

#include "entities/CEntityNPC.h"

class CEntityNPCDoor : public CEntityNPC {
private:
    std::string m_unk2D4;
    std::string m_unk2D8;
    U32 m_unk2DC;

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
};
REQUIRE_SIZE(CEntityNPCDoor, 0x2E0);

#endif
