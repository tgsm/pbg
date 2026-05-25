#include "CMailBox.h"
#include "CGame.h"
#include <cstdio>
#include <iostream>

CMailBox::CMailBox(CGame* game, U32 capacity) {
    m_game = game;
    m_capacity = capacity;
    m_message_list = NULL;

    m_message_list = new SDkMessage[m_capacity];
    m_dictionnary.clear();
    m_dictionnary.reserve(128);

    ResetMessageList();

    m_nb_active_messages = 0;
}

CMailBox::~CMailBox() {
    delete[] m_message_list;
    m_message_list = NULL;
    m_dictionnary.clear();
}

void CMailBox::ResetMessageList() {
    ResetMessageListInline();
}

void CMailBox::UpdateMessageList() {
    for (int i = 0, nb_active_messages = 0; i < m_capacity && nb_active_messages < m_nb_active_messages; i++) {
        if (m_message_list[i].flags & MESSAGE_FLAG_ACTIVE) {
            DelFlag(i, MESSAGE_FLAG_UNK1);
            DelFlag(i, MESSAGE_FLAG_UNK3);
            nb_active_messages++;
        }
    }
}

U32 CMailBox::GetSaveSize() {
    if (GetNumberOfActiveMessages() == 0) {
        return 0;
    }

    m_dictionnary.clear();
    m_dictionnary.reserve(128);

    AddActiveMessagesToDictionnary();
    U32 size = m_dictionnary.size() * 16 + 12; // FIXME: Calculate these
    size += GetNumberOfActiveMessages() * 4;
    return size;
}

// Equivalent: regalloc
BOOL CMailBox::SendMessage(std::string from, std::string to, std::string type, U32 param) {
    U32 index = 0;
    BOOL found_empty_slot = FALSE;
    if (from.size() > 15 || from.size() == 0 || to.size() > 15 || to.size() == 0 || type.size() > 15 || type.size() == 0) {
        char buf[512];
        sprintf(buf, "CMailBox : Invalid Message Sended\n FROM:%s - TO:%s - TYPE:%s - PARAM:%d\n Message might exceeds 15 characters or is empty !",
                from.c_str(), to.c_str(), type.c_str(), param);
        return FALSE;
    }

    if (ExistMessage(from, to, type)) {
        return TRUE;
    }

    while (index < m_capacity && !found_empty_slot) {
        if (!(m_message_list[index].flags & MESSAGE_FLAG_ACTIVE)) {
            found_empty_slot = TRUE;
            continue;
        }
        index++;
    }

    if (!found_empty_slot) {
        SDkMessage* messages = new SDkMessage[m_capacity + 16];
        memcpy(messages, m_message_list, m_capacity * sizeof(SDkMessage));

        int i;
        SDkMessage* message;
        for (i = m_capacity, message = &messages[i]; i < m_capacity + 16; message++, i++) {
            message->flags = 0;
            sprintf(message->to, "");
            sprintf(message->from, "");
            sprintf(message->type, "");
            message->param = 0;
        }
        delete[] m_message_list;
        m_message_list = messages;
        m_capacity += 16;

        return SendMessage(from, to, type, param);
    } else {
        sprintf(m_message_list[index].from, from.c_str());
        sprintf(m_message_list[index].to, to.c_str());
        sprintf(m_message_list[index].type, type.c_str());
        m_message_list[index].param = param;
        AddFlag(index, MESSAGE_FLAG_UNK1 | MESSAGE_FLAG_ACTIVE);
        DelFlag(index, MESSAGE_FLAG_UNK3 | MESSAGE_FLAG_UNK2);
        m_nb_active_messages++;
        return TRUE;
    }
}

BOOL CMailBox::GetMessage(SDkMessage* message, std::string& a1, BOOL remove) {
    int i = 0;
    BOOL found = FALSE;
    int nb_active_messages = 0;

    for (; i < m_capacity && !found && nb_active_messages < m_nb_active_messages; i++) {
        if (m_message_list[i].flags & MESSAGE_FLAG_ACTIVE) {
            nb_active_messages++;
            if (a1 == m_message_list[i].to && !(m_message_list[i].flags & MESSAGE_FLAG_UNK3)) {
                found = TRUE;

                sprintf(message->from, m_message_list[i].from);
                sprintf(message->to, m_message_list[i].to);
                sprintf(message->type, m_message_list[i].type);
                message->param = m_message_list[i].param;
                message->flags = m_message_list[i].flags;
                AddFlag(i, MESSAGE_FLAG_UNK3);

                if (remove) {
                    m_message_list[i].flags = 0;
                    m_nb_active_messages--;
                }
            }
        }
    }

    return found;
}

BOOL CMailBox::ExistMessage(std::string from, std::string to, std::string type) {
    int i = 0;
    BOOL exists = FALSE;

    for (; i < m_capacity && !exists; i++) {
        if (m_message_list[i].flags & MESSAGE_FLAG_ACTIVE) {
            if ((from.size() == 0 || from == m_message_list[i].from) && (to.size() == 0 || to == m_message_list[i].to) && (type.size() == 0 || type == m_message_list[i].type)) {
                exists = TRUE;
            }
        }
    }

    return exists;
}

U32 CMailBox::GetDictionnaryIndexFromString(std::string str) {
    U32 index = -1;
    U32 i = 0;
    BOOL found = FALSE;

    for (; i < m_dictionnary.size() && !found; i++) {
        if (std::string(m_dictionnary[i]) == str) {
            found = TRUE;
            index = i;
        }
    }

    return index;
}

U32 CMailBox::AddToDictionnary(std::string str) {
    U32 index = GetDictionnaryIndexFromString(str);
    if (index == -1) {
        m_dictionnary.push_back(str);
        index = m_dictionnary.size() - 1;
    }
    return index;
}
