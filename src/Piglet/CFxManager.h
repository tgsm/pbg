#ifndef PIGLET_CFXMANAGER_H
#define PIGLET_CFXMANAGER_H

#include <string>
#include "engine/wrap/DKW_V3d.h"

class CGame;

class CFxElt {
public:
    U8 m_unk0[0x18];
public:
    void StopFx();
};
REQUIRE_SIZE(CFxElt, 0x18);

// TODO
class CFxManager {
public:
    U8 m_unk0[0x14];

public:
    CFxManager(CGame* game);
    ~CFxManager();

    void Clear();
    void Render();
    CFxElt* AddFx(const std::string& xmd_path, F32, const CDKW_V3d& position, int);
};
REQUIRE_SIZE(CFxManager, 0x14);

#endif
