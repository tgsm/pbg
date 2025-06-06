#ifndef PIGLET_ENTITIES_CENTITYNPCGRALOSAURUS_H
#define PIGLET_ENTITIES_CENTITYNPCGRALOSAURUS_H

#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntityNPC.h"

class CEntityNPCGralosaurus : public CEntityNPC {
private:
    F32 m_unk2D4;
    CDKW_V3d m_unk2D8;
    CDKW_V3d m_unk2E4;
    CDKW_V3d m_unk2F0;
    int m_unk2FC;

public:
    CEntityNPCGralosaurus(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCGralosaurus();

    virtual U32 GetType() {
        return ENTITY_NPC_GRALOSAURUS;
    }
    virtual void Init();
    virtual void Reset();
    virtual void UpdatePursuitBehaviour(F32);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
    virtual void Render(F32 dt_maybe);
    virtual void Restore(void*);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void PlayWalkAnim(int);
    virtual void Idle();
    virtual void UnIdle(int);

    BOOL MoveAlongZ(F32);
};
REQUIRE_SIZE(CEntityNPCGralosaurus, 0x300);

#endif
