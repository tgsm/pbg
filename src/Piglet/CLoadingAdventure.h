#ifndef PIGLET_CLOADINGADVENTURE_H
#define PIGLET_CLOADINGADVENTURE_H

#include "CLoading.h"

class CGame;

class CLoadingAdventure : public CLoadingCallback {
public:
    int m_hero_state;

public:
    CLoadingAdventure(CGame* game);
    virtual ~CLoadingAdventure();

    virtual void Create();
    virtual void Destroy();
    virtual void Update();
    virtual void SetHeroState(int state) {
        m_hero_state = state;
    }

    inline void LookAtInline(DKDSP::IFramable* framable, CDKW_V3d vec1, CDKW_V3d vec2, CDKW_V3d vec3) {
        ((CDKW_Matrix*)(&framable->GetFrame()->m_rwframe->modelling))->LookAt(vec1, vec2, vec3);
        RwFrameUpdateObjects(framable->GetFrame()->m_rwframe);
    }

    inline void UpdateModelPos(DKDSP::IFramable* framable, CDKW_V3d pos) {
        RwMatrix* model = &framable->GetFrame()->m_rwframe->modelling;
        model->pos = pos;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(framable->GetFrame()->m_rwframe);
    }
};
REQUIRE_SIZE(CLoadingAdventure, 0x48);

#endif
