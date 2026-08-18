#include "entities/CEntityBhvTagSendMessage.h"
#include <cstring>
#include <iostream>

CEntityBhvTagSendMessage::CEntityBhvTagSendMessage() {
    m_receiver = "";
    m_type = "";
    m_param = 0;
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
                m_receiver = dest.GetStringValue();
            } else if (strcmp(buf, "Type") == 0) {
                m_type = dest.GetStringValue();
            } else if (strcmp(buf, "Param") == 0) {
                m_param = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
