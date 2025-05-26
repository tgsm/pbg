#ifndef ENGINE_DISPLAY_ICLUMP_H
#define ENGINE_DISPLAY_ICLUMP_H

#include "engine/wrap/DKW_Frame.h"

namespace DKDSP {

class IClump {
private:
    u8 m_unk0[4]; // FIXME: This is part of IFramable
public:
    virtual CDKW_Frame* GetFrame() = 0;
    // TODO
};

}

#endif
