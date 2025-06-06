#include "entities/CEntityBhvTagGuiCtrl.h"
#include <cstring>
#include <iostream>

CEntityBhvTagGuiCtrl::CEntityBhvTagGuiCtrl() {

}

CEntityBhvTagGuiCtrl::~CEntityBhvTagGuiCtrl() {

}

void CEntityBhvTagGuiCtrl::Parse(DkXmd::CChunkIterator iter) {
    char buf1[128];
    char buf2[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf1, dest.GetName());

            if (strcmp(buf1, "Name") == 0) {
                m_name = dest.GetStringValue();
            } else if (strcmp(buf1, "Activation") == 0) {
                strcpy(buf2, dest.GetStringValue());

                if (strcmp(buf2, "Off") == 0) {
                    m_active = FALSE;
                } else {
                    m_active = TRUE;
                }
            } else if (strcmp(buf1, "Visibility") == 0) {
                strcpy(buf2, dest.GetStringValue());

                if (strcmp(buf2, "Off") == 0) {
                    m_visible = FALSE;
                } else {
                    m_visible = TRUE;
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
