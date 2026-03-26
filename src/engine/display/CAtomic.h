#ifndef ENGINE_DISPLAY_CATOMIC_H
#define ENGINE_DISPLAY_CATOMIC_H

#include "engine/display/CGeometry.h"
#include "engine/display/IAtomic.h"
#include "engine/wrap/DKW_Atomic.h"

namespace DKDSP {

// TODO
class CAtomic : public IAtomic {
public:
    CDKW_Atomic* m_wrap_atomic;
    CGeometry m_geometry;

public:
    CAtomic() {
        m_wrap_atomic = NULL;
    }
    ~CAtomic();

    virtual CDKW_Frame* GetFrame();
    virtual void SetFrame(CDKW_Frame* frame);
    virtual U32 GetSize();
    virtual CGeometry* GetGeometry();
    virtual int GetNumberOfMaterials();
};
REQUIRE_SIZE(CAtomic, 0x14);

}

#endif
