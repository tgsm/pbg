#ifndef PIGLET_ENTITIES_CENTITYNPCTREE_H
#define PIGLET_ENTITIES_CENTITYNPCTREE_H

#include "entities/CEntityNPC.h"

class CEntityNPCTree : public CEntityNPC {
public:
    CEntityNPCTree(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCTree();

    virtual u32 GetType() {
        return ENTITY_NPC_TREE;
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
    virtual void PlayWalkAnim(int);
    virtual void UpdateDeathBehaviour(f32);

    BOOL funny_check() {
        BOOL ret = FALSE;

        if (m_animation_star_controller->IsPlayingAnimationLooped()) {
            ret = TRUE;
        }
        if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "FRIGHTEN_PIGLET") != 0) {
            ret = FALSE;
        }

        return ret;
    }
};

#endif
