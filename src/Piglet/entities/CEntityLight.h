#ifndef PIGLET_ENTITIES_CENTITYLIGHT_H
#define PIGLET_ENTITIES_CENTITYLIGHT_H

#include <vector>
#include "engine/display/CLight.h"
#include "engine/display/CShadowMap.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "entities/CEntityObject.h"

class CEntityMesh;

struct SShadowParams {
    DKDSP::CLight* light;
    DKDSP::CShadowMap* shadow_map;
    CEntityMesh* unk8;
    CDKW_RGBAReal color;
};

class CEntityLight : public CEntityObject {
public:
    std::vector<SShadowParams> m_shadow_params;
    DKDSP::CLight* m_light4C;

public:
    CEntityLight(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityLight();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_LIGHT; }
    virtual void Reset();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual CDKW_V3d GetPosition() {
        CDKW_Frame* frame = m_light4C->GetFrame();
        return (CDKW_V3d&)frame->m_rwframe->modelling.pos;
    }
    virtual void SetPosition(CDKW_V3d& position);
    virtual void SetOrientation(CDKW_V3d& orientation);
    virtual CDKW_Matrix GetMatrix() {
        CDKW_Frame* frame = m_light4C->GetFrame();
        return (CDKW_Matrix&)frame->m_rwframe->modelling;
    }
    virtual BOOL Create(std::string type);
    virtual DKDSP::CLight* GetLight();
};
REQUIRE_SIZE(CEntityLight, 0x50);

#endif
