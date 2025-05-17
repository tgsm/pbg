#include "entities/CEntityBhvTagSettings.h"
#include <cstring>
#include <iostream>

CEntityBhvTagSettings::CEntityBhvTagSettings() {
    m_unk14 = 0;
}

CEntityBhvTagSettings::~CEntityBhvTagSettings() {

}

void CEntityBhvTagSettings::Parse(DkXmd::CChunkIterator iter) {
    char buf1[128];
    char buf2[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf1, dest.GetName());

            if (strcmp(buf1, "Name") == 0) {
                m_unkC = dest.GetStringValue();
            } else if (strcmp(buf1, "Value") == 0) {
                if (m_unkC == "SoundVolume") {
                    m_unk10 = dest.GetFloatValue();
                } else if (m_unkC == "Vibration") {
                    strcpy(buf2, dest.GetStringValue());
                    if (strcmp(buf2, "Off") == 0) {
                        m_unk10 = 0.0f;
                    } else {
                        m_unk10 = 1.0f;
                    }
                } else if (m_unkC == "Stereo") {
                    strcpy(buf2, dest.GetStringValue());
                    if (strcmp(buf2, "Off") == 0) {
                        m_unk10 = 0.0f;
                    } else {
                        m_unk10 = 1.0f;
                    }
                } else if (m_unkC == "ShowMinimap") {
                    m_unk10 = dest.GetFloatValue();
                } else if (m_unkC == "HideMinimap") {
                    m_unk10 = dest.GetFloatValue();
                }
            } else if (strcmp(buf1, "Id") == 0) {
                m_unk14 |= (dest.GetS32Value() << 16);
            } else if (strcmp(buf1, "Room") == 0) {
                m_unk14 |= dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
