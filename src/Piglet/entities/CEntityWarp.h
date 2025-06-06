#ifndef PIGLET_ENTITIES_CENTITYWARP_H
#define PIGLET_ENTITIES_CENTITYWARP_H

#include "engine/display/CWarp.h"
#include "entities/CEntity.h"

class CEntityWarp : public CEntity {
private:
    DKDSP::CWarp* m_warp;

public:
    CEntityWarp(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityWarp();

    virtual U32 GetType() {
        return ENTITY_WARP;
    }
    virtual U32 GetSaveSize() {
        return 0;
    }

    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);

    virtual DKDSP::CWarp* GetWarp() {
        return m_warp;
    }
};
REQUIRE_SIZE(CEntityWarp, 0x28);

#endif
