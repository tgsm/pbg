#ifndef ENGINE_DISPLAY_IEVENTANIMATION_H
#define ENGINE_DISPLAY_IEVENTANIMATION_H

#include "engine/display/IGenericAnimation.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

// FIXME: Should this be here?
struct SEVENTANIMKEY {
    F32 unk0;
    int id;
    DkXmd::CChunkIterator m_chunk_iterator;

    SEVENTANIMKEY() {
        unk0 = 0.0f;
        id = -1;
    }
};

class IEventAnimation : public IGenericAnimation {
public:
    IEventAnimation();

    virtual U32 GetNumberOfEvents() = 0;
};

}

#endif
