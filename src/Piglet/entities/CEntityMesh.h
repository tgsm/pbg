#ifndef PIGLET_ENTITIES_CENTITYMESH_H
#define PIGLET_ENTITIES_CENTITYMESH_H

#include "engine/display/CAnimationStarController.h"
#include "engine/display/CClump.h"
#include "engine/display/CController.h"
#include "engine/display/CMirror.h"
#include "engine/display/IShadowMapValidationCallback.h"
#include "engine/sound/CSample.h"
#include "engine/sound/CSoundEmitter.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntityObject.h"
#include "entities/CEntityLight.h"

class CEntityMeshShadowMapValidationCallback : public DKDSP::IShadowMapValidationCallback {
public:
    BOOL m_active;

public:
    CEntityMeshShadowMapValidationCallback() { m_active = TRUE; }
    ~CEntityMeshShadowMapValidationCallback() {}

    virtual BOOL ValidateDisplay(DKDSP::IShadowMap*, DKDSP::ICamera*, CDKW_Atomic*, CDKW_Sphere*);
    virtual BOOL ValidateObjectDisplay(CDKW_Atomic*);
};

struct SAttachedFX {
    // TODO
};

struct SAttachedSND {
    int id;
    U8 unk4[8];
};

class CEntityMesh : public CEntityObject {
public: // protected
    CEntityMeshShadowMapValidationCallback m_smv_callback;
    std::vector<SAttachedFX> m_attached_fxs;
    std::vector<SAttachedSND> m_attached_snds;
    CDKW_RGBAReal m_unk60;
    CDKW_RGBAReal m_unk70;
    CEntityLight* m_entity_light_01;
    DKDSP::CMirror* m_mirror;
    DKSND::CSoundEmitter* m_sound_emitter;
    DKDSP::CClump* m_clump;
    DkPh::RBody m_unk90;
    U8 m_unkB0[0xBC - 0xB0];
    F32 m_unkBC;
    DKDSP::CController* m_controller;
    DKDSP::CAnimationStarController* m_animation_star_controller;
    void* m_unkC8; // CAnimationStar*
    F32 m_unkCC;
    F32 m_unkD0;
    F32 m_unkD4;
    F32 m_unkD8;
    F32 m_unkDC;
    F32 m_unkE0;
    F32 m_unkE4;
    F32 m_unkE8;
    BOOL m_unkEC;
    BOOL m_unkF0;

public:
    CEntityMesh(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityMesh();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_MESH; }
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize() { return CEntity::GetSaveSize(); }
    virtual CDKW_V3d GetPosition() { return m_clump->GetFrame()->m_rwframe->modelling.pos; }
    virtual void SetPosition(CDKW_V3d& position);
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual F32 GetYOrientation();
    virtual void SetYOrientation(F32 y);
    virtual CDKW_Matrix GetMatrix();
    virtual U32 Create(std::string mesh_filename);
    virtual void RenderAttachedFX(F32);
    virtual void BeginLighting();
    virtual void EndLighting();
    virtual void UpdateAttachedFX(F32, S32);
    virtual void UpdateCollisions(F32, S32);
    virtual void UpdateAnimations(F32 dt);
    virtual void OrientToDirection(CDKW_V3d&, F32, F32);

    void PlayAnimAnm(DKDSP::IGenericAnimation* animation, F32);
    void PlayAnimEvn(DKDSP::IGenericAnimation* animation, F32);
    void PlayAnimDma(DKDSP::IGenericAnimation* animation, U32);
    void PlayAnimTan(DKDSP::IGenericAnimation* animation, U32);
    void LoadAnimations(DkXmd::CChunkIterator iter);
    void ParseStar(std::string);
    void StopAttachedFX(int id);
    void StartAttachedFX(int id);
    void ResumeAllAttachedFX();
    void PauseAllAttachedFX();
    void StopAllAttachedFX();
    DKSND::CSample* GetSampleById(int id);
    F32 GetPitchVariationById(int id);
    BOOL ParseMirror(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityMesh, 0xF4);

#endif
