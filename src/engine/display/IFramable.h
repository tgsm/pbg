#ifndef ENGINE_DISPLAY_IFRAMABLE_H
#define ENGINE_DISPLAY_IFRAMABLE_H

#include "engine/wrap/DKW_Frame.h"

namespace DKDSP {

class IFramable {
private:
    U8 m_unk0[4];

public:
    IFramable();

    virtual CDKW_Frame* GetFrame() = 0;
    virtual void SetFrame(CDKW_Frame* frame) = 0;
};

}

#endif
