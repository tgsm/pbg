#ifndef ENGINE_DISPLAY_CCONTROLLERANIMENTRY_H
#define ENGINE_DISPLAY_CCONTROLLERANIMENTRY_H

#include "engine/display/CHierarchy.h"

namespace DKDSP {

// TODO
class CControllerAnimEntry {
public:
    U8 m_unk0[0x28 - 0x0];
    F32 m_unk28;
    U8 m_unk2C[0x48 - 0x2C];
    int m_unk48;
    U8 m_unk4C[0x54 - 0x4C];
    CHierarchy* m_unk54;
    U8 m_unk58[0x60 - 0x58];
};
REQUIRE_SIZE(CControllerAnimEntry, 0x60);

}

#endif
