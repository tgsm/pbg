#pragma once on

#include "entities/CEntityBhvTagData.h"
#include "entities/CEntity.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagMessage : public CEntityBhvTagData {
public:
    std::string m_unkC;
    std::string m_unk10;
    u32 m_unk14; // unknown type

public:
    CEntityBhvTagMessage();
    virtual ~CEntityBhvTagMessage();

    virtual void Set(CEntity* entity) {
        for (CEntityBhvTagData* bhv = m_unk4; bhv != NULL; bhv = bhv->m_unk4) {
            if (bhv->GetType() == 5) {
                bhv->Set(entity);
            }
        }
    }

    virtual u32 GetType() {
        return 4;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagMessage, 0x18);
