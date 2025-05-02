#pragma once on

#include "entities/CEntityHero.h"

class CEntityPushingBox;

class CEntityPiglet : public CEntityHero {
public:
    u8 m_unk260[0x278 - 0x260];
    f32 m_unk278;
    u32 m_unk27C;
    f32 m_unk280;
    int m_unk284;
    u8 m_unk288[0x2A0 - 0x288];
    u32 m_unk2A0;
    u8 m_unk2A4[0x32C - 0x2A4];

public:
    virtual ~CEntityPiglet();

    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual u32 GetType() { return ENTITY_PIGLET; }
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual u32 GetSaveSize();
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ManageMessage(SDkMessage&);
    virtual void ResolveContact(); // FIXME: params
    virtual void UpdateAnimations(f32);
    virtual void SetMode(u32 mode);
    virtual void DicreaseLife(int);
    virtual void UpdateActions();
    virtual void UpdateMoving(f32);
    virtual void AddItem(char*);
    virtual void RemoveItem(char*);
    virtual void RemoveItem(int);
    virtual void SetStateEnvironement();
    virtual void SetGrimace(int grimace);
    virtual void HideOSD();
    virtual void UnHideOSD(u32);
    virtual void UpdateOSD(f32);
    virtual void RenderOSD();
    virtual void PlayOSDSound(std::string name, int);
    virtual void PushStart(CEntityPushingBox* box);
    virtual void PushPush(CEntityPushingBox* box);
    virtual void PushBloc(CEntityPushingBox* box);
    virtual void PushStop(CEntityPushingBox* box);
};
REQUIRE_SIZE(CEntityPiglet, 0x32C);
