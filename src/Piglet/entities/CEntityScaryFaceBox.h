#ifndef PIGLET_ENTITIES_CENTITYSCARYFACEBOX_H
#define PIGLET_ENTITIES_CENTITYSCARYFACEBOX_H

#include "engine/display/CIm2DBatch.h"
#include "entities/CEntityPiglet.h"
#include "entities/CEntityZone.h"

class CEntityScaryFaceBox : public CEntityZone {
public:
    std::string m_unk1BC;
    std::string m_unk1C0;
    U32 m_nb_needed_cookies;
    U8 m_unk1C8[0x1D8 - 0x1C8];
    F32 m_unk1D8;
    int m_unk1DC;

    static DKDSP::CIm2DBatch* m_pImediatePolyBack;
    static DKDSP::CIm2DBatch* m_pImediatePolyIcon;

public:
    CEntityScaryFaceBox(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityScaryFaceBox();

    virtual U32 GetType() { return ENTITY_SCARY_FACE_BOX; }
    virtual void Init();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void ManageMessage(SDkMessage& message);
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual BOOL ShouldKick();

    void RenderCookiesNbIcon(F32 dt);
    static void RenderCookiesNbIconDirect(CGame* game, int, F32 x, F32 y, int, F32 width, F32 height, int, int);

    int NbCookiesForNextGrimaceLevel() {
        CEntityPiglet* piglet = (CEntityPiglet*)m_entity_manager->GetHero();
        switch (piglet->m_grimace_level) {
            case 1:
                return 50;
            case 2:
                return 70;
            case 3:
                return 80;
            case 4:
                return 90;
            case 5:
                return 100;
            case 6:
                return 120;
            case 7:
                return 130;
            case 8:
                return 140;
            case 9:
                return 150;
            default:
                return 0;
        }
    }

    int NbCookiesForNextGrimaceLevel2() {
        CEntityPiglet* piglet = (CEntityPiglet*)m_entity_manager->GetHero();
        switch (piglet->m_grimace_level - 1) {
            case 1:
                return 50;
            case 2:
                return 70;
            case 3:
                return 80;
            case 4:
                return 90;
            case 5:
                return 100;
            case 6:
                return 120;
            case 7:
                return 130;
            case 8:
                return 140;
            case 9:
                return 150;
            default:
                return 0;
        }
    }

    void HandleAction() {
        std::string str;
        m_unk1D8 = 0.0f;
        BOOL cond = FALSE;
        F32 cookies = m_entity_manager->GetGame()->GetCurrentMission().GetMaxCollectedNbCookies();

        if (cookies >= NbCookiesForNextGrimaceLevel()) {
            cond = TRUE;
        }

        if (cond == TRUE) {
            str.assign(m_unk1C0, 0);
            m_unk1DC = 1;

            CMission::m_MaxNbCookies -= NbCookiesForNextGrimaceLevel();

            DelFlag(1 << 0);
            DelFlag(1 << 1);
            DelFlag(1 << 2);
            AddFlag(1 << 6);

            ((CEntityPiglet*)m_entity_manager->GetHero())->m_grimace_level++;
        } else {
            str.assign(m_unk1BC, 0);
            m_unk1DC = 0;
        }

        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, str, "START", 0);
    }
};
REQUIRE_SIZE(CEntityScaryFaceBox, 0x1E0);

#endif
