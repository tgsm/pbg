#ifndef PIGLET_CLOADINGWINNIE_H
#define PIGLET_CLOADINGWINNIE_H

#include "CLoading.h"

class CGame;

class CLoadingWinnie : public CLoadingCallback {
public:
    CLoadingWinnie(CGame* game);
    virtual ~CLoadingWinnie();

    virtual void Create();
    virtual void Destroy();
    virtual void Update();
};
REQUIRE_SIZE(CLoadingWinnie, 0x44);

#endif
