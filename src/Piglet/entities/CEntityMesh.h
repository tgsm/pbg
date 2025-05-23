#ifndef PIGLET_ENTITIES_CENTITYMESH_H
#define PIGLET_ENTITIES_CENTITYMESH_H

#include "engine/display/CAnimationStarController.h"
#include "engine/display/CController.h"
#include "entities/CEntityObject.h"

class CEntityMesh : public CEntityObject {
public: // protected
    u8 m_unk40[4];
    BOOL m_unk44;
    u8 m_unk48[0xC0 - 0x48];
    DKDSP::CController* m_controller;
    DKDSP::CAnimationStarController* m_animation_star_controller;
    u8 m_unkC8[0xF4 - 0xC8];

public:
    CEntityMesh(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityMesh();

    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual u32 GetType();
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual u32 GetSaveSize();
    virtual CDKW_V3d GetPosition();
    virtual void SetPosition(CDKW_V3d& position);
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual f32 GetYOrientation();
    virtual void SetYOrientation(f32 y);
    virtual CDKW_Matrix GetMatrix();
    virtual u32 Create(std::string);
    virtual void RenderAttachedFX(f32);
    virtual void BeginLighting();
    virtual void EndLighting();
    virtual void UpdateAttachedFX(f32, s32);
    virtual void UpdateCollisions(f32, s32);
    virtual void UpdateAnimations(f32);
    virtual void OrientToDirection(CDKW_V3d&, f32, f32);
};
REQUIRE_SIZE(CEntityMesh, 0xF4);

#endif
