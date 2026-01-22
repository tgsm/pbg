#ifndef PIGLET_CLOADINGCATCHTHEMALL_H
#define PIGLET_CLOADINGCATCHTHEMALL_H

#include "engine/display/CDMorphAnimation.h"
#include "CLoading.h"

class CGame;

class CLoadingCatchThemAll : public CLoadingCallback {
public:
    DKDSP::CClump* m_clump44;
    DKDSP::CAnimation* m_anims48[2];
    DKDSP::CDMorphAnimation* m_anims50[2];
    int m_unk58;

public:
    CLoadingCatchThemAll(CGame* game);
    virtual ~CLoadingCatchThemAll();

    virtual void Create();
    virtual void Destroy();
    virtual void Update();
    virtual void UpdateObjects(F32 dt);
    virtual void RenderObjects();
};
REQUIRE_SIZE(CLoadingCatchThemAll, 0x5C);

#endif
