#ifndef PIGLET_CRTCCAMERA_H
#define PIGLET_CRTCCAMERA_H

#include <string>
#include "engine/display/CAnimation.h"
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityManager.h"
#include "CRTCSequencer.h"

class CRTCCamera : public CRTCSeqKey {
private:
    F32 m_fov;
    std::string m_cam_filename;
    DKDSP::CAnimation* m_unk18;
    BOOL m_loaded;

public:
    CRTCCamera();
    virtual ~CRTCCamera();

    BOOL CreateFromFile(DkXmd::CChunkIterator* iter);
    void Load(CEntityManager* entity_manager);
    void UnLoad(CEntityManager* entity_manager);
};
REQUIRE_SIZE(CRTCCamera, 0x20);

#endif
