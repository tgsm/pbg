#pragma once on

#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntity.h"

// TODO
class CDKW_Matrix {};

class CEntityObject : public CEntity {
private:
    u32 m_unk24;
    u32 m_unk28;
    void* m_unk2C;
    f32 m_unk30;
    f32 m_unk34;
    f32 m_unk38;
    f32 m_unk3C;

public:
    CEntityObject(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityObject();

    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual u32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual u32 GetSaveSize();
    virtual CDKW_V3d GetPosition();
    virtual void SetPosition(CDKW_V3d& position) {}
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual f32 GetYOrientation();
    virtual void SetYOrientation(f32 y);
    virtual CDKW_Matrix GetMatrix();
    virtual void UpdateContacts();
    virtual void ResolveContact(); // FIXME: params
    virtual void UpdateVolumes();
    virtual void ParseCollisions(DkXmd::CChunkIterator iter);
    virtual void ParseCollisionsBVolume(DkXmd::CChunkIterator iter); // FIXME: params
    virtual void ParseCollisionsBVolumeOBox(DkXmd::CChunkIterator iter); // FIXME: params
};
REQUIRE_SIZE(CEntityObject, 0x40);
