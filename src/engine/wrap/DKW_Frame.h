#ifndef ENGINE_WRAP_DKW_FRAME_H
#define ENGINE_WRAP_DKW_FRAME_H

#include <rwsdk/plcore/bavector.h>
#include <rwsdk/baframe.h>
#include "types.h"

class CDKW_Frame {
public:
    virtual ~CDKW_Frame();
    virtual RwFrame* GetRwFrame();

public:
    RwFrame* m_rwframe;
}; // Unknown size

#endif
