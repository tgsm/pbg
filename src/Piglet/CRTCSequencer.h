#ifndef PIGLET_CRTCSEQUENCER_H
#define PIGLET_CRTCSEQUENCER_H

#include "types.h"

// TODO
class CRTCSeqKey {
protected:
    F32 m_unk0;
    F32 m_unk4;
    U32 m_unk8;

public:
    CRTCSeqKey();
    virtual ~CRTCSeqKey();

    virtual void Activate();
    virtual void Reinitialize();
};
REQUIRE_SIZE(CRTCSeqKey, 0x10);

#endif
