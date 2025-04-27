#include "entities/CEntityBhvTagSendMessage.h"
#include <cstring>
#include <iostream>

CEntityBhvTagSendMessage::CEntityBhvTagSendMessage() {
    m_unkC = "";
    m_unk10 = "";
    m_unk14 = 0;
}

CEntityBhvTagSendMessage::~CEntityBhvTagSendMessage() {
    
}

void CEntityBhvTagSendMessage::Parse(DkXmd::CChunkIterator iter) {
    DkXmd::CChunkIterator dest;
    char buf[128];
    
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());
            if (strcmp(buf, "Receiver") == 0) {
                m_unkC = dest.GetStringValue();
            } else if (strcmp(buf, "Type") == 0) {
                m_unk10 = dest.GetStringValue();
            } else if (strcmp(buf, "Param") == 0) {
                m_unk14 = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
