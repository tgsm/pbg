#ifndef PIGLET_ENTITIES_CENTITYOBJECT_H
#define PIGLET_ENTITIES_CENTITYOBJECT_H

#include "engine/xmd/CChunkIterator.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntity.h"

// FIXME: Move this to the engine
namespace DkPh {

struct RBody {
    RBody() : unk0(0), unk4(0), unk8(0.0f, 0.0f, 0.0f), unk14(0.0f, 0.0f, 0.0f) {

    }

    ~RBody();

    u32 unk0;
    u32 unk4;
    CDKW_V3d unk8;
    CDKW_V3d unk14;
};

class Collider {
public:
    class Body {};
};

};

// TODO
class CDKW_Matrix {};

class CEntityObject : public CEntity {
private:
    U32 m_unk24;
    U32 m_unk28;
    void* m_unk2C;
    F32 m_unk30;
    F32 m_unk34;
    F32 m_unk38;
    F32 m_unk3C;

public:
    CEntityObject(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityObject();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize();
    virtual CDKW_V3d GetPosition();
    virtual void SetPosition(CDKW_V3d& position) {}
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual F32 GetYOrientation();
    virtual void SetYOrientation(F32 y);
    virtual CDKW_Matrix GetMatrix();
    virtual void UpdateContacts();
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateVolumes();
    virtual void ParseCollisions(DkXmd::CChunkIterator iter);
    virtual void ParseCollisionsBVolume(DkXmd::CChunkIterator iter); // FIXME: params
    virtual void ParseCollisionsBVolumeOBox(DkXmd::CChunkIterator iter); // FIXME: params
};
REQUIRE_SIZE(CEntityObject, 0x40);

#endif
