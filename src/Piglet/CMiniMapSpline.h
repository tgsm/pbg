#ifndef PIGLET_CMINIMAPSPLINE_H
#define PIGLET_CMINIMAPSPLINE_H

#include <vector>
#include "engine/display/CSpline.h"
#include "engine/xmd/CChunkIterator.h"

class CGame;

class CMiniMapSpline {
public:
    std::string m_unk0;
    DKDSP::CSpline* m_spline;

public:
    DKDSP::CSpline* GetSpline() { return m_spline; }
};

// TODO
class CMiniMapSplineMgr {
public:
    std::vector<CMiniMapSpline> m_unk0;
    CGame* m_game;

public:
    CMiniMapSplineMgr(CGame* game);
    ~CMiniMapSplineMgr();

    BOOL Parse(DkXmd::CChunkIterator iter);
    CMiniMapSpline* GetSpline(std::string a1);
    void ClearList();
};
REQUIRE_SIZE(CMiniMapSplineMgr, 0x10);

#endif
