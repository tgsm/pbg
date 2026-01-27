#ifndef PIGLET_CMINIMAPSPLINE_H
#define PIGLET_CMINIMAPSPLINE_H

#include <vector>
#include "engine/display/CSpline.h"
#include "types.h"

class CGame;

class CMiniMapSpline {
public:
    std::string m_unk0;
    DKDSP::CSpline* m_spline;
};

// TODO
class CMiniMapSplineMgr {
public:
    std::vector<CMiniMapSpline> m_unk0;
    CGame* m_game;

public:
    CMiniMapSplineMgr(CGame* game);
    ~CMiniMapSplineMgr();

    CMiniMapSpline* GetSpline(std::string a1);
    void ClearList();
};
REQUIRE_SIZE(CMiniMapSplineMgr, 0x10);

#endif
