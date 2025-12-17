#ifndef ENGINE_DISPLAY_CDKWRAPENGINE_H
#define ENGINE_DISPLAY_CDKWRAPENGINE_H

#include <rwsdk/badevice.h>
#include "engine/wrap/DKW_GCNEngine.h"

namespace DKDSP {

class CDkWrapEngine : public CDKW_GCNEngine {
public:
    void* m_unk20;
    RwRect m_rect;
    U32 m_unk34;
    U8 m_unk38[0x48 - 0x38];
    U32 m_unk48;
    U8 m_unk4C[0x50 - 0x4C];

public:
    CDkWrapEngine(RwEngineOpenParams* params, void* a1);
    virtual ~CDkWrapEngine();

    virtual BOOL OnPlugAttach();
    virtual BOOL OnResize(RwRect* rect);
    virtual BOOL OnInit(int argc, char** argv);
    virtual BOOL OnIdle();
    virtual BOOL OnTerm();

    BOOL Update();
    RwRect GetRect();

    static BOOL m_bEnableShowWindow;
};
REQUIRE_SIZE(CDkWrapEngine, 0x50);

}

#endif
