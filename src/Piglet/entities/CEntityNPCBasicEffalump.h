#ifndef PIGLET_ENTITIES_CENTITYNPCBASICEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCBASICEFFALUMP_H

#include <math.h>
#include "entities/CEntityNPC.h"

class CEntityNPCBasicEffalump : public CEntityNPC {
private:
    f32 m_animation_speed;
    f32 m_unk2D8;

public:
    CEntityNPCBasicEffalump(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCBasicEffalump();

    virtual u32 GetType() {
        return ENTITY_NPC_BASIC_EFFALUMP;
    }

    virtual void Reset();
    virtual void UpdateDetectionBehaviour(f32);
    virtual void UpdateFightBehaviour(f32);
    virtual void UpdateGrimaceBehaviour(f32);
    virtual void Render(f32);
    virtual void Restore(void*);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
};
REQUIRE_SIZE(CEntityNPCBasicEffalump, 0x2DC);

#endif
