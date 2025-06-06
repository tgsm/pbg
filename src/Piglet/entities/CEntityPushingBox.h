#ifndef PIGLET_ENTITIES_CENTITYPUSHINGBOX_H
#define PIGLET_ENTITIES_CENTITYPUSHINGBOX_H

#include "entities/CEntityMesh.h"

class CEntityPushingBox : public CEntityMesh {
public:
    U8 m_unkF4[0x198 - 0xF4];

public:
    CEntityPushingBox(CEntityManager* entity_manager, std::string name);

    // FIXME: Stop being lazy and fill these in.
    // There are also overrides above these that need to be put in.
    virtual void func38();
    virtual void func39();
    virtual void func40();
    virtual void func41();
    virtual void func42();
    virtual void func43();
    virtual void func44();
    virtual void func45();
    virtual void SetBoxPosition(int x, int y);
    virtual void func47();
    virtual void DeleteCurrentBoxPosition();
    virtual void func49();
    virtual void func50();
    virtual void func51();
    virtual void func52();
    virtual void func53();
    virtual void func54();
    virtual void func55();
    virtual void func56();
    virtual void func57();
};
REQUIRE_SIZE(CEntityPushingBox, 0x198);

#endif
