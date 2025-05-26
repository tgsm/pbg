#ifndef PIGLET_ENTITIES_CENTITYNPCMIRRORWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCMIRRORWOOZLE_H

#include <math.h>
#include "entities/CEntityNPC.h"

class CEntityNPCMirrorWoozle : public CEntityNPC {
private:
    f32 m_unk2D4;
    f32 m_unk2D8;
    f32 m_unk2DC;
    f32 m_unk2E0;
    f32 m_unk2E4;
    int m_unk2E8;
    BOOL m_unk2EC;

public:
    CEntityNPCMirrorWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCMirrorWoozle();

    virtual u32 GetType() {
        return ENTITY_NPC_MIRROR_WOOZLE;
    }

    virtual void Reset();
    virtual void UpdateDetectionBehaviour(f32);
    virtual void UpdateFightBehaviour(f32);
    virtual void UpdateGrimaceBehaviour(f32);
    virtual void UpdateDeathBehaviour(f32);
    virtual void Render(f32);
    virtual void Restore(void*);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
};
REQUIRE_SIZE(CEntityNPCMirrorWoozle, 0x2F0);

#endif
