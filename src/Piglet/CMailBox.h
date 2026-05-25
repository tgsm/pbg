#ifndef PIGLET_CMAILBOX_H
#define PIGLET_CMAILBOX_H

#include <string>
#include <vector>
#include "types.h"

#define MESSAGE_FLAG_ACTIVE (1 << 0)
#define MESSAGE_FLAG_UNK1   (1 << 1)
#define MESSAGE_FLAG_UNK2   (1 << 2)
#define MESSAGE_FLAG_UNK3   (1 << 3)

class CGame;

struct SDkMessage {
    char from[16];
    char to[16];
    char type[16];
    U32 param;
    U32 flags;
};
REQUIRE_SIZE(SDkMessage, 0x38);

// TODO
class CMailBox {
private:
    SDkMessage* m_message_list;
    U32 m_capacity;
    CGame* m_game;
    std::vector<std::string> m_dictionnary;
    int m_nb_active_messages;

public:
    CMailBox(CGame* game, U32 capacity);
    ~CMailBox();

    void ResetMessageList();
    void UpdateMessageList();
    U32 GetSaveSize();
    void SaveMessageList(void* data_ptr);
    BOOL RestoreMessageList(void* data_ptr);
    BOOL SendMessage(std::string from, std::string to, std::string type, U32 param);
    BOOL ExistMessage(std::string from, std::string to, std::string type);
    BOOL GetMessage(SDkMessage* message, std::string& a1, BOOL remove); // Does this return void?
    U32 GetDictionnaryIndexFromString(std::string str);
    U32 AddToDictionnary(std::string str);

    virtual void AddFlag(U32 index, U32 flag) {
        m_message_list[index].flags |= flag;
    }
    virtual void DelFlag(U32 index, U32 flag) {
        m_message_list[index].flags &= ~flag;
    }

    void ResetMessageListInline() {
        for (U32 i = 0; i < m_capacity; i++) {
            m_message_list[i].flags = 0;
            sprintf(m_message_list[i].to, "");
            sprintf(m_message_list[i].from, "");
            sprintf(m_message_list[i].type, "");
            m_message_list[i].param = 0;
        }
    }

    U32 GetNumberOfActiveMessages() {
        U32 nb_active_messages = 0;
        for (int i = 0; i < m_capacity; i++) {
            if (m_message_list[i].flags & MESSAGE_FLAG_ACTIVE) {
                nb_active_messages++;
            }
        }
        return nb_active_messages;
    }

    void AddActiveMessagesToDictionnary() {
        for (int i = 0; i < m_capacity; i++) {
            if (m_message_list[i].flags & MESSAGE_FLAG_ACTIVE) {
                AddToDictionnary(m_message_list[i].from);
                AddToDictionnary(m_message_list[i].to);
                AddToDictionnary(m_message_list[i].type);
            }
        }
    }
};
REQUIRE_SIZE(CMailBox, 0x20);

#endif
