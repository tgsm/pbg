#ifndef PIGLET_ENTITIES_CENTITYMESH_H
#define PIGLET_ENTITIES_CENTITYMESH_H

#include "engine/display/CAnimationStarController.h"
#include "engine/display/CClump.h"
#include "engine/display/CController.h"
#include "entities/CEntityObject.h"

class CEntityMesh : public CEntityObject {
public: // protected
    U8 m_unk40[4];
    BOOL m_unk44;
    U8 m_unk48[0x8C - 0x48];
    DKDSP::CClump* m_clump;
    U8 m_unk90[0xC0 - 0x90];
    DKDSP::CController* m_controller;
    DKDSP::CAnimationStarController* m_animation_star_controller;
    U8 m_unkC8[0xF4 - 0xC8];

public:
    CEntityMesh(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityMesh();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize();
    virtual CDKW_V3d GetPosition();
    virtual void SetPosition(CDKW_V3d& position);
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual F32 GetYOrientation();
    virtual void SetYOrientation(F32 y);
    virtual CDKW_Matrix GetMatrix();
    virtual U32 Create(std::string);
    virtual void RenderAttachedFX(F32);
    virtual void BeginLighting();
    virtual void EndLighting();
    virtual void UpdateAttachedFX(F32, S32);
    virtual void UpdateCollisions(F32, S32);
    virtual void UpdateAnimations(F32);
    virtual void OrientToDirection(CDKW_V3d&, F32, F32);
};
REQUIRE_SIZE(CEntityMesh, 0xF4);

#endif
