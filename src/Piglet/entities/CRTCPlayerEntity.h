#ifndef PIGLET_ENTITIES_CRTCPLAYERENTITY_H
#define PIGLET_ENTITIES_CRTCPLAYERENTITY_H

#include "entities/CEntity.h"

class CRTCPlayerEntity : public CEntity {
public:
    U8 m_unk24[0x50 - 0x24];
    F32 m_grimace_duration;
    F32 m_grimace_delay;
    U8 m_unk58[0x108 - 0x58];

public:
    CRTCPlayerEntity(CEntityManager* entity_manager, std::string name);

    BOOL IsInFade();
    void StartFade(U32);
};
REQUIRE_SIZE(CRTCPlayerEntity, 0x108);

#endif
