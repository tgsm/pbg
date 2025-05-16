#ifndef PIGLET_ENTITIES_CENTITYWINNIE_H
#define PIGLET_ENTITIES_CENTITYWINNIE_H

#include "entities/CEntityHero.h"

class CEntityWinnie : public CEntityHero {
public:
    CEntityWinnie(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityWinnie();

    virtual u32 GetType() { return ENTITY_WINNIE; }
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(f32);
    virtual void SetMode(u32 mode);
    virtual void DicreaseLife(int);
    virtual void UpdateMoving(f32);
};
REQUIRE_SIZE(CEntityWinnie, 0x260);

#endif
