#ifndef PIGLET_CMINIMAPSPLINE_H
#define PIGLET_CMINIMAPSPLINE_H

#include "types.h"

class CGame;

// TODO
class CMiniMapSplineMgr {
public:
    U8 m_unk0[0x10];

public:
    CMiniMapSplineMgr(CGame* game);
    ~CMiniMapSplineMgr();

    void ClearList();
};
REQUIRE_SIZE(CMiniMapSplineMgr, 0x10);

#endif
