#ifndef PIGLET_CLOADINGTIGGER_H
#define PIGLET_CLOADINGTIGGER_H

#include "CLoading.h"

class CGame;

class CLoadingTigger : public CLoadingCallback {
public:
    CLoadingTigger(CGame* game);
};

#endif
