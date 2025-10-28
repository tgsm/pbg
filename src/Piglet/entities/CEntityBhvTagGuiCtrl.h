#ifndef PIGLET_ENTITIES_CENTITYBHVTAGGUICTRL_H
#define PIGLET_ENTITIES_CENTITYBHVTAGGUICTRL_H

#include <string>
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntity.h"
#include "entities/CEntityBhvTagData.h"
#include "CGame.h"

class CEntityBhvTagGuiCtrl : public CEntityBhvTagData {
public:
    std::string m_name;
    BOOL m_active;
    BOOL m_visible;

public:
    CEntityBhvTagGuiCtrl();
    virtual ~CEntityBhvTagGuiCtrl();

    virtual void Set(CEntity* entity) {
        if (entity == NULL) {
            return;
        }

        // FIXME: Can't we just use ==?
        // Also this operator function ends up being emitted in the wrong place.
        if (std::operator==(m_name, "AUTOSAVE")) {
            CGame* game = entity->m_entity_manager->GetGame();
            game->m_unk8 |= (game->m_unk8 | (1 << 7));
        } else if (std::operator==(m_name, "SAVE_CHECK_MMC")) {
            entity->m_entity_manager->GetGame()->m_gui_manager->GetGuiPtr("SAVE_SAVE_READY")->menu->Reset();
            entity->m_entity_manager->GetGame()->m_gui_manager->SetActive("SAVE_SAVE_READY", 1);
            entity->m_entity_manager->GetGame()->m_gui_manager->SetVisible("SAVE_SAVE_READY", 1);
        } else {
            entity->m_entity_manager->GetGame()->m_gui_manager->GetGuiPtr(m_name)->menu->Reset();
            entity->m_entity_manager->GetGame()->m_gui_manager->SetActive(m_name, m_active);
            entity->m_entity_manager->GetGame()->m_gui_manager->SetVisible(m_name, m_visible);
        }
    }
    virtual U32 GetType() { return BEHAVIOR_TAG_GUI_CONTROL; }

    void Parse(DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CEntityBhvTagGuiCtrl, 0x18);

#endif
