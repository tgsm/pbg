#ifndef PIGLET_CLOADINGWINNIE_H
#define PIGLET_CLOADINGWINNIE_H

#include "CLoading.h"

class CGame;

class CLoadingWinnie : public CLoadingCallback {
public:
    CLoadingWinnie(CGame* game);
};

#endif
