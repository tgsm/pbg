#ifndef PIGLET_ENTITIES_CENTITYPIGLET_H
#define PIGLET_ENTITIES_CENTITYPIGLET_H

#include "entities/CEntityHero.h"
#include "entities/CEntityPushingBox.h"

class CEntityPiglet : public CEntityHero {
public:
    CDKW_V3d m_unk260;
    U32 m_unk26C; // Unknown type
    U8 m_unk270[4];
    CEntityPushingBox* m_pushing_box;
    F32 m_unk278;
    U32 m_unk27C;
    F32 m_unk280;
    int m_unk284;
    U8 m_unk288[0x2A0 - 0x288];
    U32 m_unk2A0;
    U8 m_unk2A4[0x32C - 0x2A4];

public:
    CEntityPiglet(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityPiglet();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_PIGLET; }
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual U32 GetSaveSize();
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual void UpdateAnimations(F32);
    virtual void SetMode(U32 mode);
    virtual void DicreaseLife(int);
    virtual void UpdateActions();
    virtual void UpdateMoving(F32);
    virtual void AddItem(char*);
    virtual void RemoveItem(char*);
    virtual void RemoveItem(int);
    virtual void SetStateEnvironement();
    virtual void SetGrimace(int grimace);
    virtual void HideOSD();
    virtual void UnHideOSD(U32);
    virtual void UpdateOSD(F32);
    virtual void RenderOSD();
    virtual void PlayOSDSound(std::string name, int);
    virtual void PushStart(CEntityPushingBox* box);
    virtual void PushPush(CEntityPushingBox* box);
    virtual void PushBloc(CEntityPushingBox* box);
    virtual void PushStop(CEntityPushingBox* box);
};
REQUIRE_SIZE(CEntityPiglet, 0x32C);

#endif
