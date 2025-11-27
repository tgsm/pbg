#ifndef PIGLET_CLOADINGCATCHTHEMALL_H
#define PIGLET_CLOADINGCATCHTHEMALL_H

#include "CLoading.h"

class CGame;

class CLoadingCatchThemAll : public CLoadingCallback {
public:
    U8 m_unk44[0x5C - 0x44];

public:
    CLoadingCatchThemAll(CGame* game);
};
REQUIRE_SIZE(CLoadingCatchThemAll, 0x5C);

#endif
