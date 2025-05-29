#ifndef PIGLET_CMAILBOX_H
#define PIGLET_CMAILBOX_H

#include <string>
#include "types.h"

struct SDkMessage;

// TODO
class CMailBox {
private:
    u8 m_unk0[0x20];

public:
    ~CMailBox();

    void SendMessage(std::string a0, std::string a1, std::string a2, u32 a3);
    int GetMessage(SDkMessage* message, std::string& a1, int a2); // Does this return void?
};
REQUIRE_SIZE(CMailBox, 0x20);

#endif
