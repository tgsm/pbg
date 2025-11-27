#ifndef PIGLET_CLOADINGADVENTURE_H
#define PIGLET_CLOADINGADVENTURE_H

#include "CLoading.h"

class CGame;

class CLoadingAdventure : public CLoadingCallback {
public:
    U8 m_unk44[4];

public:
    CLoadingAdventure(CGame* game);
};
REQUIRE_SIZE(CLoadingAdventure, 0x48);

#endif
