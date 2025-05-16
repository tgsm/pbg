#ifndef PIGLET_ENTITIES_CENTITYTIGGER_H
#define PIGLET_ENTITIES_CENTITYTIGGER_H

#include "entities/CEntityHero.h"

class CEntityTigger : public CEntityHero {
private:
    f32 m_unk260;
    BOOL m_unk264;

public:
    CEntityTigger(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityTigger();

    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual u32 GetType() { return ENTITY_TIGGER; }
    virtual void Update(f32 dt_maybe);
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(f32);
    virtual void SetMode(u32 mode);
    virtual void DicreaseLife(int);
    virtual void UpdateActions();
    virtual void UpdateMoving(f32);
    virtual void ReInit();
    virtual void Detected();
    virtual void UnDetected();
};
REQUIRE_SIZE(CEntityTigger, 0x268);

#endif
