#ifndef PIGLET_ENTITIES_CENTITYPHIAL_H
#define PIGLET_ENTITIES_CENTITYPHIAL_H

#include "entities/CEntityMesh.h"
#include "CFxManager.h"

class CEntityPhial : public CEntityMesh {
public:
    int m_state;
    CEntity* m_unkF8; // is this actually an entity?
    U8 m_unkFC;
    F32 m_unk100;
    F32 m_unk104;
    CFxElt* m_unk108;

public:
    CEntityPhial(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityPhial();

    virtual U32 GetSaveSize() { return 0; }
    virtual U32 GetType() { return ENTITY_PHIAL; }
    virtual void Reset();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
};
REQUIRE_SIZE(CEntityPhial, 0x10C);

#endif
