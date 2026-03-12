#ifndef PIGLET_ENTITIES_CENTITYWORLD_H
#define PIGLET_ENTITIES_CENTITYWORLD_H

#include "engine/display/CClump.h"
#include "engine/display/CController.h"
#include "engine/display/IShadowMapValidationCallback.h"
#include "entities/CEntityObject.h"

class CEntityWorldShadowMapValidationCallback : public DKDSP::IShadowMapValidationCallback {
public:
    ~CEntityWorldShadowMapValidationCallback() {}

    virtual BOOL ValidateDisplay(DKDSP::IShadowMap*, DKDSP::ICamera*, CDKW_Atomic*, CDKW_Sphere*);
    virtual BOOL ValidateObjectDisplay(CDKW_Atomic*);
};

class CEntityWorld : public CEntityObject {
public:
    DKDSP::CClump* m_clump;
    DKDSP::CController* m_controller;
    BOOL m_unk48;
    F32 m_unk4C;

public:
    CEntityWorld(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityWorld();

    virtual U32 GetType() { return ENTITY_WORLD; }
    virtual void Reset();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual U32 GetSaveSize() { return 0; }
    virtual CDKW_V3d GetPosition() {
        CDKW_Frame* frame = m_clump->GetFrame();
        return (CDKW_V3d&)(frame->m_rwframe->modelling.pos);
    }
    virtual void SetPosition(CDKW_V3d& position) {
        m_clump->UpdateModelPosRef(position);
    }
    virtual void SetOrientation(CDKW_V3d& orientation) {
        m_clump->UnkTRTInline(&CDKW_V3d::ZERO, 0.0f, 0);
        m_clump->UnkTRTInline(&CDKW_V3d::XAXIS, orientation.x, 2);
        m_clump->UnkTRTInline(&CDKW_V3d::YAXIS, orientation.y, 2);
        m_clump->UnkTRTInline(&CDKW_V3d::ZAXIS, orientation.z, 2);
    }
    virtual CDKW_Matrix GetMatrix() {
        CDKW_Frame* frame = m_clump->GetFrame();
        return (CDKW_Matrix&)(frame->m_rwframe->modelling);
    }
    virtual DKDSP::CClump* GetClump() { return m_clump; }
};
REQUIRE_SIZE(CEntityWorld, 0x50);

#endif
