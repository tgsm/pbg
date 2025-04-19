#include "entities/CEntityBhvTagTeleport.h"
#include <cstring>
#include <iostream>

CEntityBhvTagTeleport::CEntityBhvTagTeleport() {

}

CEntityBhvTagTeleport::~CEntityBhvTagTeleport() {

}

void CEntityBhvTagTeleport::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    m_no_fade = FALSE;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "NOFADE") == 0) {
                m_no_fade = TRUE;
            }

            if (strcmp(buf, "Room") == 0) {
                m_room = dest.GetS32Value();
            } else if (strcmp(buf, "Hero") == 0) {
                m_hero = dest.GetS32Value();
            } else if (strcmp(buf, "HeroPosition") == 0) {
                DkXmd::CChunkIterator pos_chunk;
                if (dest.GetChunk("X", pos_chunk)) {
                    m_hero_pos_x = pos_chunk.GetS32Value();
                }
                if (dest.GetChunk("Y", pos_chunk)) {
                    m_hero_pos_y = pos_chunk.GetS32Value();
                }
                if (dest.GetChunk("Z", pos_chunk)) {
                    m_hero_pos_z = pos_chunk.GetS32Value();
                }
            } else if (strcmp(buf, "HeroRotation") == 0) {
                DkXmd::CChunkIterator rot_chunk;
                if (dest.GetChunk("X", rot_chunk)) {
                    m_hero_rot_x = rot_chunk.GetS32Value();
                }
                if (dest.GetChunk("Y", rot_chunk)) {
                    m_hero_rot_y = rot_chunk.GetS32Value();
                }
                if (dest.GetChunk("Z", rot_chunk)) {
                    m_hero_rot_z = rot_chunk.GetS32Value();
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
