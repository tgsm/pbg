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
#include "CGame.h"

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
    DKDSP::CParticleEmitter* emitter;
    int id;
    int bone_link;
    U32 follow_rotation;
    F32 x;
    F32 y;
    F32 z;

    SAttachedFX() {
        bone_link = -1;
        emitter = NULL;
        id = -1;
        follow_rotation = 0;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
};

struct SAttachedSND {
    int id;
    DKSND::CSample* sample;
    F32 pitch_variation;

    SAttachedSND() {
        sample = NULL;
        id = -1;
        pitch_variation = 0.0f;
    }
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
    DKDSP::CAnimationStar* m_animation_star;
    F32 m_cone_angle;
    F32 m_cone_distance;
    F32 m_cone_mv_ratio;
    F32 m_cone_offset;
    F32 m_unkDC;
    F32 m_unkE0;
    F32 m_unkE4;
    F32 m_unkE8;
    BOOL m_look_at_cam;
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
    virtual CDKW_V3d GetPosition() { return m_clump->GetPosition(); }
    virtual void SetPosition(CDKW_V3d& position);
    virtual CDKW_V3d GetOrientation();
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual F32 GetYOrientation();
    virtual void SetYOrientation(F32 y);
    virtual CDKW_Matrix GetMatrix() {
        CDKW_Frame* frame = m_clump->GetFrame();
        return (CDKW_Matrix&)(frame->m_rwframe->modelling);
    }
    virtual U32 GetSaveSize() { return CEntity::GetSaveSize(); }
    virtual U32 Create(std::string mesh_filename);
    virtual void RenderAttachedFX(F32);
    virtual void BeginLighting();
    virtual void EndLighting();
    virtual void UpdateAttachedFX(F32, S32);
    virtual void UpdateCollisions(F32, S32);
    virtual void UpdateAnimations(F32 dt);
    virtual void OrientToDirection(CDKW_V3d&, F32, F32);

    void ParseAttachedFX(DkXmd::CChunkIterator iter) {
        SAttachedFX fx;
        DkXmd::CChunkIterator dest;

        if (iter.GetChunk("BoneLink", dest)) {
            fx.bone_link = dest.GetS32Value();
        }

        if (iter.GetChunk("FollowRotation", dest)) {
            fx.follow_rotation = dest.GetS32Value();
        }

        if (iter.GetChunk("Id", dest)) {
            fx.id = dest.GetS32Value();
        }

        if (iter.GetChunk("Offset", dest)) {
            F32 x, y, z;
            ParseXYZ(dest, &x, &y, &z);
            fx.x = x;
            fx.y = y;
            fx.z = z;
        }

        if (iter.GetChunk("ParticleEmitterFile", dest)) {
            if (dest.GetStringValue() != NULL) {
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION, dest.GetStringValue());
                fx.emitter = m_entity_manager->GetGame()->GetScene()->CreateParticleEmitterFromDefinition("", m_entity_manager->GetGame()->GetObjectDictionary()->FindParticleEmitterDefinition(dest.GetStringValue()));
            }
            if (fx.emitter != NULL) {
                fx.emitter->Stop();
            }
        }

        m_attached_fxs.push_back(fx);
    }

    void ParseAttachedSND(DkXmd::CChunkIterator iter) {
        SAttachedSND sound;
        DkXmd::CChunkIterator dest;

        if (m_sound_emitter == NULL) {
            m_sound_emitter = m_entity_manager->GetGame()->m_sound_engine->CreateEmitter();
            if (m_sound_emitter != NULL) {
                m_sound_emitter->SetName(&m_unk0);
            }
        }

        if (iter.GetChunk("Id", dest)) {
            sound.id = dest.GetS32Value();
        }

        if (iter.GetChunk("SoundId", dest)) {
            std::string str;
            str = dest.GetStringValue();
            sound.sample = m_entity_manager->GetGame()->m_sample_dictionary->FindSample(&str, 1);
        }

        if (iter.GetChunk("PitchVariation", dest)) {
            sound.pitch_variation = dest.GetFloatValue();
        }

        m_attached_snds.push_back(sound);
    }

    void PlayAnimAnm(DKDSP::IGenericAnimation* animation, F32);
    void PlayAnimEvn(DKDSP::IGenericAnimation* animation, F32);
    void PlayAnimDma(DKDSP::IGenericAnimation* animation, U32);
    void PlayAnimTan(DKDSP::IGenericAnimation* animation, U32);
    void LoadAnimations(DkXmd::CChunkIterator iter);
    BOOL ParseStar(std::string);
    BOOL ParseStar(DkXmd::CChunkIterator iter);
    BOOL IsViewedWithoutY(CDKW_V3d&, int);
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
