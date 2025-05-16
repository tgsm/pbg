#ifndef PIGLET_ENTITIES_CENTITYNPCARROW_H
#define PIGLET_ENTITIES_CENTITYNPCARROW_H

#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntityMesh.h"

class CEntityNPCArrow : public CEntityMesh {
private:
    u8 m_unkF4[0x100 - 0xF4];
    CDKW_V3d m_unk100;
    CDKW_V3d m_unk10C;

public:
    CEntityNPCArrow(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCArrow();

    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void SetPosition(CDKW_V3d& position);

    virtual u32 GetSaveSize() {
        return 0;
    }

    virtual u32 GetType() {
        return ENTITY_NPC_ARROW;
    }
};
REQUIRE_SIZE(CEntityNPCArrow, 0x118);

#endif
