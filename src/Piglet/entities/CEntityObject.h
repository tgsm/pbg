#ifndef PIGLET_ENTITIES_CENTITYOBJECT_H
#define PIGLET_ENTITIES_CENTITYOBJECT_H

#include <vector>
#include "engine/physics/DkPh_Collider.h"
#include "engine/physics/DkPh_Primitives.h"
#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_Matrix.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntity.h"

class CEntityObject : public CEntity {
protected:
    std::vector<DkPh::BVolume> m_unk24;
    F32 m_unk30;
    F32 m_unk34;
    F32 m_unk38;
    F32 m_unk3C;

public:
    CEntityObject(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityObject();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_OBJECT; }
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize() { return CEntity::GetSaveSize(); }
    virtual CDKW_V3d GetPosition() { return CDKW_V3d::ZERO; }
    virtual void SetPosition(CDKW_V3d& position) {}
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation) {}
    virtual F32 GetYOrientation();
    virtual void SetYOrientation(F32 y);
    virtual CDKW_Matrix GetMatrix() { return CDKW_Matrix(CDKW_V3d::ZERO, CDKW_V3d::ZERO, CDKW_V3d::ZERO, CDKW_V3d::ZERO); }
    virtual void UpdateContacts();
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateVolumes();
    virtual void ParseCollisions(DkXmd::CChunkIterator iter);
    virtual void ParseCollisionsBVolume(DkXmd::CChunkIterator iter, DkPh::BVolume*);
    virtual void ParseCollisionsBVolumeOBox(DkXmd::CChunkIterator iter, DkPh::BVolumeOBB*);
};
REQUIRE_SIZE(CEntityObject, 0x40);

#endif
