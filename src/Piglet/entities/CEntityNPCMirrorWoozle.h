#ifndef PIGLET_ENTITIES_CENTITYNPCMIRRORWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCMIRRORWOOZLE_H

#include <math.h>
#include "entities/CEntityNPC.h"

class CEntityNPCMirrorWoozle : public CEntityNPC {
private:
    F32 m_unk2D4;
    F32 m_unk2D8;
    F32 m_unk2DC;
    F32 m_unk2E0;
    F32 m_unk2E4;
    int m_unk2E8;
    BOOL m_unk2EC;

public:
    CEntityNPCMirrorWoozle(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCMirrorWoozle();

    virtual U32 GetType() {
        return ENTITY_NPC_MIRROR_WOOZLE;
    }

    virtual void Reset();
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
    virtual void Render(F32);
    virtual void Restore(void*);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
};
REQUIRE_SIZE(CEntityNPCMirrorWoozle, 0x2F0);

#endif
