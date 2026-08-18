#ifndef ENGINE_WRAP_DKW_SPLINE_H
#define ENGINE_WRAP_DKW_SPLINE_H

#include <rwsdk/plcore/bamemory.h>
#include "engine/wrap/DKW_Frame.h"
#include "engine/wrap/DKW_Node3d.h"

struct RpSpline;

// TODO
class CDKW_Spline : public CDKW_Node3d {
public:
    RpSpline* m_rw_spline;
    CDKW_Frame* m_frame; // Not entirely confident this is actually a frame. Only NULL is written to it.

    static RwFreeList* ms_pMemEntry;

    virtual RwFrame* GetRwFrame() { return m_frame->m_rwframe; }

public:
    CDKW_Spline(U32);
};
REQUIRE_SIZE(CDKW_Spline, 0xC);

#endif
