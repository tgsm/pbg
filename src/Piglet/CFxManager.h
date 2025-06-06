#ifndef PIGLET_CFXMANAGER_H
#define PIGLET_CFXMANAGER_H

class CGame;

// TODO
class CFxManager {
public:
    U8 m_unk0[0x14];

public:
    CFxManager(CGame* game);
    ~CFxManager();

    void Clear();
    void Render();
};
REQUIRE_SIZE(CFxManager, 0x14);

#endif
