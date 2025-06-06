#ifndef PIGLET_CMAILBOX_H
#define PIGLET_CMAILBOX_H

#include <string>
#include <vector>
#include "types.h"

class CGame;
struct SDkMessage;

struct MailBoxItem { // Unknown name, may just be SDkMessage
    char unk0[16];
    char unk10[16];
    char unk20[16];
    U32 unk30;
    U32 flags;
};
REQUIRE_SIZE(MailBoxItem, 0x38);

// TODO
class CMailBox {
private:
    MailBoxItem* m_items;
    U32 m_max_items;
    CGame* m_game;
    std::vector<std::string> m_unkC;
    int m_unk18;

public:
    CMailBox(CGame* game, U32 max_items);
    ~CMailBox();

    void ResetMessageList();
    void UpdateMessageList();
    void SendMessage(std::string a0, std::string a1, std::string a2, U32 a3);
    int GetMessage(SDkMessage* message, std::string& a1, int a2); // Does this return void?
    U32 GetDictionnaryIndexFromString(std::string);
    U32 AddToDictionnary(std::string);

    virtual void AddFlag(U32 index, U32 flag) {
        m_items[index].flags |= flag;
    }
    virtual void DelFlag(U32 index, U32 flag) {
        m_items[index].flags &= ~flag;
    }
};
REQUIRE_SIZE(CMailBox, 0x20);

#endif
