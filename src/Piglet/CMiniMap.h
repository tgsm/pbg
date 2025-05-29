#ifndef PIGLET_CMINIMAP_H
#define PIGLET_CMINIMAP_H

#include "engine/xmd/CChunkIterator.h"

class CGame;

// TODO
class CMiniMap {
public:
    u8 m_unk0[0xB4];

public:
    CMiniMap(CGame* game);
    ~CMiniMap();

    BOOL Load(DkXmd::CChunkIterator*);
    BOOL Unload();
    void ShowFriendNPC(int, int, int);
    void HideFriendNPC(int);
};
REQUIRE_SIZE(CMiniMap, 0xB4);

#endif
