#ifndef ENGINE_DISPLAY_IATOMIC_H
#define ENGINE_DISPLAY_IATOMIC_H

#include "engine/display/CGeometry.h"
#include "engine/display/IFramableGeometry.h"

namespace DKDSP {

// TODO
class IAtomic : public IFramableGeometry {
public:
    virtual U32 GetSize() = 0;
    virtual CGeometry* GetGeometry() = 0;
    virtual int GetNumberOfMaterials() = 0;
};

}

#endif
