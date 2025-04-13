#include "CMission.h"
#include <cstring>

u32 CMission::m_MaxNbCookies = 0;

u32 CMission::GetSaveSize() {
    // FIXME: Calculate this
    return 0x298;
}

void CMission::Initialize() {
    m_game = NULL;
    m_mission_no = -1;

    SetName("");
    SetEndMissionFMV("");
    SetStartMissionFMV("");
    SetStartRTC("");
    SetMissionFightWarpModel("");

    m_unk10 = 0;
    m_unkC = 1;

    m_num_rooms = 1;

    m_start_position_x = 0.0f;
    m_start_position_y = 0.0f;
    m_start_position_z = 0.0f;
    m_start_rotation_x = 0.0f;
    m_start_rotation_y = 0.0f;
    m_start_rotation_z = 0.0f;
    m_unk2C = 0;
    m_unk30 = 0;

    strcpy(m_unk48C[0], "NULL");
    strcpy(m_unk48C[1], "NULL");
    strcpy(m_unk48C[2], "NULL");

    m_num_total_cookies = 0;
    m_num_current_cookies = 0;

    m_MaxNbCookies = 0;

    m_unk34 = 0;

    m_unk4C4 = 0;
    m_unk4C8 = 0;
    m_unk4CC = 0;
    m_unk4D0 = 0;

    for (int i = 0; i < 16; i++) {
        m_rooms[i] = 0;
    }

    // FIXME: Initialize m_unk7C
}

int CMission::AddItem(s8* str_) {
    int ret;

    char* str = (char*)str_;

    if (strcmp(str, m_unk48C[0]) == 0) {
        ret = 0;
    } else if (strcmp(str, m_unk48C[1]) == 0) {
        ret = 1;
    } else if (strcmp(str, m_unk48C[2]) == 0) {
        ret = 2;
    } else if (strcmp("NULL", m_unk48C[0]) == 0) {
        strcpy(m_unk48C[0], str);
        ret = 0;
    } else if (strcmp("NULL", m_unk48C[1]) == 0) {
        strcpy(m_unk48C[1], str);
        ret = 1;
    } else if (strcmp("NULL", m_unk48C[2]) == 0) {
        strcpy(m_unk48C[2], str);
        ret = 2;
    } else {
        ret = -1;
    }

    return ret;
}

void CMission::RemoveItem(s8* str_) {
    char* str = (char*)str_;

    if (strcmp(str, m_unk48C[0]) == 0) {
        strcpy(m_unk48C[0], "NULL");
    } else if (strcmp(str, m_unk48C[1]) == 0) {
        strcpy(m_unk48C[1], "NULL");
    } else if (strcmp(str, m_unk48C[2]) == 0) {
        strcpy(m_unk48C[2], "NULL");
    }
}

void CMission::RemoveItem(int item_no) {
    strcpy(m_unk48C[item_no], "NULL");
}

char* CMission::GetItem(int item_no) {
    if (strcmp("NULL", m_unk48C[item_no]) == 0) {
        return NULL;
    }
    return m_unk48C[item_no];
}

u32 CMission::GetNbTotalCookies() {
    return m_num_total_cookies;
}

u32 CMission::GetCurrentNbCookies() {
    return m_num_current_cookies;
}

u32 CMission::GetMaxCollectedNbCookies() {
    return m_MaxNbCookies;
}

BOOL CMission::IsRoomCompleted(u32 room_no) {
    return (m_rooms[room_no] & 2) != 0;
}

BOOL CMission::IsRoomOpened(u32 room_no) {
    return (m_rooms[room_no] & 1) != 0;
}

BOOL CMission::IsSecondPassCompleted() {
    BOOL ret = TRUE;

    for (int i = 1; i <= m_num_rooms; i++) {
        ret = IsRoomCompleted(i) ? TRUE : FALSE;
        if (ret == FALSE) {
            break;
        }
    }

    return ret;
}
