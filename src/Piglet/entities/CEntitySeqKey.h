#ifndef PIGLET_ENTITIES_CENTITY_SEQKEY_H
#define PIGLET_ENTITIES_CENTITY_SEQKEY_H

#include "entities/CEntityObject.h"

class CEntitySeqKey : public CEntityObject {
public:
    U8 m_unk40[0x54 - 0x40];
    F32 m_unk54;
    U8 m_unk58[0x14908 - 0x58];

public:
    CEntitySeqKey(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntitySeqKey, 0x14908);

#endif
