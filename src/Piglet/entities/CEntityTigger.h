#ifndef PIGLET_ENTITIES_CENTITYTIGGER_H
#define PIGLET_ENTITIES_CENTITYTIGGER_H

#include "entities/CEntityHero.h"

class CEntityTigger : public CEntityHero {
private:
    F32 m_unk260;
    BOOL m_unk264;

public:
    CEntityTigger(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityTigger();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_TIGGER; }
    virtual void Update(F32 dt_maybe);
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(F32);
    virtual void SetMode(U32 mode);
    virtual void DicreaseLife(int);
    virtual void UpdateActions();
    virtual void UpdateMoving(F32);
    virtual void ReInit();
    virtual void Detected();
    virtual void UnDetected();
};
REQUIRE_SIZE(CEntityTigger, 0x268);

#endif
