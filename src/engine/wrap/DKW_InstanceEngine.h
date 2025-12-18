#ifndef ENGINE_WRAP_DKW_INSTANCEENGINE_H
#define ENGINE_WRAP_DKW_INSTANCEENGINE_H

#include "engine/wrap/DKW_Frame.h"

// TODO
class CDKW_InstanceEngine {
public:
    static CDKW_InstanceEngine* ms_pInstanceEngine;

public:
    virtual CDKW_Frame* NewFrame(void*, U32);
};

#endif
