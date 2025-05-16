#ifndef PIGLET_ENTITIES_CRTCPLAYERENTITY_H
#define PIGLET_ENTITIES_CRTCPLAYERENTITY_H

#include "entities/CEntity.h"

class CRTCPlayerEntity : public CEntity {
private:
    u8 m_unk24[0x108 - 0x24];

public:
};
REQUIRE_SIZE(CRTCPlayerEntity, 0x108);

#endif
