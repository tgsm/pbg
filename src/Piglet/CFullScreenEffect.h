#ifndef PIGLET_CFULLSCREENEFFECT_H
#define PIGLET_CFULLSCREENEFFECT_H

#include "engine/display/IScene.h"
#include "engine/xmd/CChunkIterator.h"

class CGame;

// TODO
class CScreenEffect {
public:
    U8 m_unk0[0x78];

public:
    CScreenEffect(CGame* game);
    ~CScreenEffect();

    void Parse(DkXmd::CChunkIterator iter);
    void Update(F32 dt);
    void Render(DKDSP::IScene* scene);
    void SetSequenceByIndex(int);
};
REQUIRE_SIZE(CScreenEffect, 0x78);

#endif
