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
};
REQUIRE_SIZE(CLoadingAdventure, 0x48);

#endif
