#ifndef PIGLET_ENTITIES_CENTITYPIGLET_H
#define PIGLET_ENTITIES_CENTITYPIGLET_H

#include <rwsdk/badevice.h>
#include "engine/display/CIm2DBatch.h"
#include "engine/sound/CSound2D.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityPushingBox.h"

struct ItemsMission {
    ItemsMission();
    ~ItemsMission();

    U8 unk0[12];
};

class CEntityPiglet : public CEntityHero {
public:
    CDKW_V3d m_unk260;
    U32 m_unk26C; // Unknown type
    U32 m_unk270;
    CEntityPushingBox* m_pushing_box;
    F32 m_unk278;
    U32 m_grimace_level;
    F32 m_unk280;
    int m_unk284;
    DKDSP::CIm2DBatch* m_batches[6];
    U32 m_unk2A0;
    U8 m_unk2A4[4];
    int m_unk2A8;
    U32 m_unk2AC;
    U8 m_unk2B0[0x2B4 - 0x2B0];
    char m_unk2B4[4];
    U8 m_unk2B8[0x2D4 - 0x2B8];
    DKSND::CSound2D* m_unk2D4;
    ItemsMission m_items_missions[7];

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

    // FIXME: This may not actually exist. It matches when used in Update(), but not UpdateOSD().
    void UpdateOSD_Impl(F32 dt) {
        switch (m_unk284) {
            case 0:
                m_unk280 = 0.0f;
                return;
            case 1:
                if (m_unk280 >= 0.2f) {
                    m_unk284 = 2;
                }
                break;
            case 2:
                if ((U32)RwEngineInstance->stringFuncs.rwstrlen(m_unk2B4) != 0) {
                    m_entity_manager->GetGame()->PlayNarratorLine(m_unk2B4);
                    RwEngineInstance->stringFuncs.rwsprintf(m_unk2B4, "");
                }
                if (m_unk280 >= 2.2f) {
                    m_unk284 = 3;
                    F32 what = m_unk280 - 2.0f;
                    m_unk280 = what - 0.2f;

                    PlayOSDSound("SND_202_2D", 0);
                }
                break;
            case 3:
                if (m_unk280 >= 0.2f) {
                    m_unk280 = 0.0f;
                    m_unk284 = 0;
                    return;
                }
                break;
        }

        m_unk280 += dt;
    }
};
REQUIRE_SIZE(CEntityPiglet, 0x32C);

#endif
