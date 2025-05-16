#ifndef PIGLET_ENTITIES_CENTITYNPCDOOR_H
#define PIGLET_ENTITIES_CENTITYNPCDOOR_H

#include "entities/CEntityNPC.h"

class CEntityNPCDoor : public CEntityNPC {
private:
    std::string m_unk2D4;
    std::string m_unk2D8;
    u32 m_unk2DC;

public:
    CEntityNPCDoor(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCDoor();

    virtual u32 GetType() {
        return ENTITY_NPC_DOOR;
    }
    virtual void Reset();
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(f32);
    virtual void UpdateFightBehaviour(f32);
    virtual void UpdateGrimaceBehaviour(f32);
    virtual void UpdateSpecialBehaviour(f32);
    virtual void UpdateDeathBehaviour(f32);

    BOOL CanLaunchFightMode();
};
REQUIRE_SIZE(CEntityNPCDoor, 0x2E0);

#endif
