#ifndef PIGLET_ENTITIES_CENTITYBHVTAGSETTINGS_H
#define PIGLET_ENTITIES_CENTITYBHVTAGSETTINGS_H

#include <string>
#include <dolphin/os/OSRtc.h>
#include "engine/input/IInputEngine.h"
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntity.h"
#include "entities/CEntityBhvTagData.h"
#include "CGame.h"

class CEntityBhvTagSettings : public CEntityBhvTagData {
private:
    std::string m_unkC;
    F32 m_unk10;
    int m_unk14;

public:
    CEntityBhvTagSettings();
    virtual ~CEntityBhvTagSettings();

    virtual void Set(CEntity* entity) {
        if (entity == NULL) {
            return;
        }

        if (m_unkC == "TXDPATCH") {
            for (int i = 3; i < 11; i++) {
                entity->GetManager()->GetGame()->GetMission(3).m_rooms.rooms[i] |= (1 << 3);
            }
        } else if (m_unkC == "Vibration") {
            if (m_unk10 > 0.5f) {
                DKI::IInputEngine::GetDevice(0)->VibrationEnable();
            } else {
                DKI::IInputEngine::GetDevice(0)->StopVibration();
                DKI::IInputEngine::GetDevice(0)->VibrationDisable();
            }
        } else if (m_unkC == "SoundVolume") {
            if (m_unk10 >= 0.0f && m_unk10 <= 1.0f) {
                entity->GetManager()->GetGame()->GetSoundEngine()->SetGlobalVolume(m_unk10);
                entity->GetManager()->GetGame()->m_unk504C = m_unk10;
            }
        } else if (m_unkC == "Stereo") {
            if (m_unk10 > 0.5f) {
                entity->GetManager()->GetGame()->GetSoundEngine()->SetStereoMode(OS_SOUND_MODE_STEREO);
                OSSetSoundMode(OS_SOUND_MODE_STEREO);
            } else {
                entity->GetManager()->GetGame()->GetSoundEngine()->SetStereoMode(OS_SOUND_MODE_MONO);
                OSSetSoundMode(OS_SOUND_MODE_MONO);
            }
        } else if (m_unkC == "ShowMinimap") {
            CMiniMap* minimap = entity->GetManager()->GetGame()->GetMiniMap();
            if (minimap != NULL) {
                minimap->ShowFriendNPC(m_unk14 >> 16, m_unk10, m_unk14 & 0xFFFF);
            }
        } else if (m_unkC == "HideMinimap") {
            CMiniMap* minimap = entity->GetManager()->GetGame()->GetMiniMap();
            if (minimap != NULL) {
                minimap->HideFriendNPC(m_unk14 >> 16);
            }
        }
    }
    virtual U32 GetType() {
        return BEHAVIOR_TAG_SETTINGS;
    }

    void Parse(DkXmd::CChunkIterator iter);
};

#endif
