#ifndef ENGINE_DISPLAY_CHIERARCHY_H
#define ENGINE_DISPLAY_CHIERARCHY_H

#include "engine/display/IHierarchy.h"

namespace DKDSP {

// TODO
class CHierarchy : public IHierarchy {
public:
    ~CHierarchy();

    virtual int GetSize();
    virtual int GetNumberOfNodes();
    virtual void Update();
    virtual IHierarchy* Copy(IHierarchy*);
    virtual IHierarchy* Clone(U32);
    virtual int GetIndexFromID(int id);

    void Release();
};

}

#endif
