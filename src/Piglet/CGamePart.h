#ifndef PIGLET_CGAMEPART_H
#define PIGLET_CGAMEPART_H

#include "types.h"

// Unofficial name
enum EGamePartType {
    GAME_PART_TYPE_NONE,
    GAME_PART_TYPE_CREDITS,
    GAME_PART_TYPE_MOVIE_CLIPS,
    GAME_PART_TYPE_SCRAP_BOOK,
    GAME_PART_TYPE_START_SCREEN,
    GAME_PART_TYPE_UNK5,
    GAME_PART_TYPE_FRONTEND,
    GAME_PART_TYPE_INGAME,
    GAME_PART_TYPE_INTRODUCTION,
    GAME_PART_TYPE_DM_ROOM_LAUNCHER,
    GAME_PART_TYPE_SHUTDOWN,
};

class CGamePart {
public:
    EGamePartType m_type;
    int m_unk4;

public:
    CGamePart() {
        m_type = GAME_PART_TYPE_NONE;
    }
    virtual ~CGamePart();

    EGamePartType GetType() { return m_type; }

    virtual U32 NextFrame() = 0;
};
REQUIRE_SIZE(CGamePart, 0xC);

#endif
