#pragma once on

#include "engine/CChunkIterator.h"
#include "CEntityBhvTagData.h"

class CEntityBhvTagBehavior : public CEntityBhvTagData {
private:
    u32 m_id;

public:
    CEntityBhvTagBehavior();
    virtual ~CEntityBhvTagBehavior();

    virtual void Set(CEntity* entity);

    virtual u32 GetType() {
        return 1;
    }

    void Parse(DkXmd::CChunkIterator iter);
    CEntityBhvTagBehavior* GetBehavior(u32 id);
};
