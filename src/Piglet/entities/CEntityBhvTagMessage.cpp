#include "entities/CEntityBhvTagMessage.h"
#include "entities/CEntityBhvTagResponse.h"
#include <cstring>
#include <iostream>

CEntityBhvTagMessage::CEntityBhvTagMessage() {
    m_unkC = "";
    m_unk10 = "";
    m_unk14 = 0;
}

CEntityBhvTagMessage::~CEntityBhvTagMessage() {

}

// Equivalent: regalloc
void CEntityBhvTagMessage::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) != TRUE) {
        return;
    }

    while (TRUE) {
        strcpy(buf, dest.GetName());

        if (strcmp(buf, "Emitter") == 0) {
            m_unkC = dest.GetStringValue();
        } else if (strcmp(buf, "Type") == 0) {
            m_unk10 = dest.GetStringValue();
        } else if (strcmp(buf, "Param") == 0) {
            m_unk14 = dest.GetS32Value();
        } else if (strcmp(buf, "Response") == 0) {
            CEntityBhvTagResponse* response = new CEntityBhvTagResponse;
            response->Parse(dest);
            AddData(response);
        }

        if (dest.GetNextSiblingChunk(dest) != TRUE) {
            break;
        }
    }
}
