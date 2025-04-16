#pragma once

#include <string>
#include "types.h"

struct SDkMessage;

// TODO
class CMailBox {
public:
    void SendMessage(std::string a0, std::string a1, std::string a2, u32 a3);
    int GetMessage(SDkMessage* message, std::string& a1, int a2); // Does this return void?
};
