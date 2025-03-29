#pragma once

#include "CEntityBhvTagData.h"
#include "CEntity.h"
#include "engine/CChunkIterator.h"

class CEntityBhvTagResponse : public CEntityBhvTagData {
public:
    CEntityBhvTagResponse();
    virtual ~CEntityBhvTagResponse();

    virtual void Set(CEntity* entity) {
        for (CEntityBhvTagData* bhv = m_unk4; bhv != NULL; bhv = bhv->m_unk0) {
            bhv->Set(entity);
        }
    }

    virtual u32 GetType() {
        return 5;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
