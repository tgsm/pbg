#pragma once on

#include <string>
#include "CGame.h"
#include "types.h"

struct CMission_UnkSubstruct {
    u8 unk0[256];
};

class CMission {
private:
    CGame* m_game;
    std::string m_mission_name;
    int m_mission_no;
    u32 m_unkC;
    u32 m_unk10;
    f32 m_start_position_x;
    f32 m_start_position_y;
    f32 m_start_position_z;
    f32 m_start_rotation_x;
    f32 m_start_rotation_y;
    f32 m_start_rotation_z;
    u32 m_unk2C;
    u32 m_unk30;
    u32 m_unk34;
    int m_num_rooms;
    u32 m_rooms[16];
    CMission_UnkSubstruct m_unk7C[4];
    std::string m_start_mission_fmv;
    std::string m_end_mission_fmv;
    std::string m_mission_fight_warp_model;
    std::string m_start_rtc;
    char m_unk48C[3][16];
    u32 m_num_total_cookies;
    u32 m_num_current_cookies;
    u32 m_unk4C4;
    u32 m_unk4C8;
    u32 m_unk4CC;
    u32 m_unk4D0;

    static u32 m_MaxNbCookies;

public:
    CMission() {}
    ~CMission() {}

    void SetName(std::string name) {
        m_mission_name.assign(name, 0);
    }

    void SetEndMissionFMV(std::string name) {
        m_end_mission_fmv.assign(name, 0);
    }

    void SetStartMissionFMV(std::string name) {
        m_start_mission_fmv.assign(name, 0);
    }

    std::string GetStartRTC() {
        return m_start_rtc;
    }
    void SetStartRTC(std::string name) {
        m_start_rtc.assign(name, 0);
    }

    std::string GetMissionFightWarpModel() {
        return m_mission_fight_warp_model;
    }
    void SetMissionFightWarpModel(std::string name) {
        m_mission_fight_warp_model.assign(name, 0);
    }

    BOOL LoadConfigFile(u32);
    void UnloadConfigFile();
    u32 GetSaveSize();
    void Save(void*);
    void Restore(void*);
    void Initialize();
    int AddItem(s8* str);
    void RemoveItem(s8* str);
    void RemoveItem(int item_no);
    char* GetItem(int item_no);
    u32 GetNbTotalCookies();
    u32 GetCurrentNbCookies();
    u32 GetMaxCollectedNbCookies();
    BOOL IsRoomCompleted(u32 room_no);
    BOOL IsRoomOpened(u32 room_no);
    BOOL IsSecondPassCompleted();
};
// size: 0x4D4
