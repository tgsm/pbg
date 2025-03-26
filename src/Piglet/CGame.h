#pragma once

#include <string>
#include "CMailBox.h"

// Very TODO, there's a lot of stuff in this class
class CGame {
private:
    u8 m_unk0[0x4FB0];
    CMailBox* m_mailbox;

public:
    CMailBox* GetMailbox() { return m_mailbox; }

    void PlayNarratorLine(std::string line_id);
};
