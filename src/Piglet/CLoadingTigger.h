#ifndef PIGLET_CLOADINGTIGGER_H
#define PIGLET_CLOADINGTIGGER_H

#include "CLoading.h"

class CGame;

class CLoadingTigger : public CLoadingCallback {
public:
    CLoadingTigger(CGame* game);
    virtual ~CLoadingTigger();

    virtual void Create();
    virtual void Destroy();
    virtual void Update();
};
REQUIRE_SIZE(CLoadingTigger, 0x44);

#endif
