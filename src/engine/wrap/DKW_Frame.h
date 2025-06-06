#ifndef ENGINE_WRAP_DKW_FRAME_H
#define ENGINE_WRAP_DKW_FRAME_H

#include "types.h"

extern "C" {

struct RwV3d {
    F32 x;
    F32 y;
    F32 z;
};

// FIXME: Do this properly
struct RwFrame {
    U8 unk0[0x34];
    RwV3d unk34;
}; // Probably wrong size

}

class CDKW_Frame {
public:
    U8 m_unk0[4];
    RwFrame* m_rwframe;
}; // Unknown size

#endif
