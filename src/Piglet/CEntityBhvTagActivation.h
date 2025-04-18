#pragma once on

#include "CEntityBhvTagData.h"
#include "CEntity.h"
#include "engine/CChunkIterator.h"

extern "C" {
extern int rand(void);
}

class CEntityBhvTagActivation : public CEntityBhvTagData {
private:
    BOOL m_on;

public:
    CEntityBhvTagActivation();
    virtual ~CEntityBhvTagActivation();

    virtual void Set(CEntity* entity) {
        if (m_on == TRUE) {
            entity->AddFlag(1 << 0);
            if (GetType() > 23 && GetType() < 36) {
                // FIXME: Need CEntityMesh(?) vtable
                rand();
            }
        } else {
            entity->DelFlag(1 << 0);
        }
    }

    virtual u32 GetType() {
        return 9;
    }

    void Parse(DkXmd::CChunkIterator iter);
};
